#ifndef CODE_TRANSLATOR_H_
#define CODE_TRANSLATOR_H_



/**
@file CodeTranslator.h
*/


#include <string>
#include <list>

#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>

#include "Value.h"
#include "Command.h"
#include "DataKeeper.h"
#include "LabelOperand.h"


namespace qi = boost::spirit::qi;


/**
@class CodeTranslator
@brief Осуществляет транслирование секции .code.
*/


class CodeTranslator
{
	public:
/**
Конструктор по-умолчанию.
*/
		CodeTranslator();
/**
Деструктор.
*/
		~CodeTranslator();

/**
Установка указателя на набор данных, полученных при трансляции секции .var.
@param pkeeper - указатель на устанавливаемый набор
*/
		void setDataKeeperPtr(DataKeeper *pkeeper);


/**
Трансляция строки.
@param str - транслируемая строка
*/
		void translate(std::string str);


/**
Получение кода операции оттранслированной строки.
@return код операции
*/
		Command::Operation getOperation() const;


/**
Получение списка имен меток, находившихся в оттранслированной строке.
@return список имён меток
*/
		std::list<std::string> getLabelsList() const;


/**
Получение команды, определённой в оттранслированной строке
*/

		Command getCommand() const;

	private:



/**
@class CodeGrammar
@brief Реализует грамматику для секции .code 
*/

	template <typename Iterator>
	class CodeGrammar: public qi::grammar<Iterator>
	{
	
		public:
/**
Конструктор. Реализует грамматику.
*/
			CodeGrammar(): CodeGrammar::base_type(expression_operation)
			{
				m_pcommand = NULL;
				m_plbls = NULL;

                expression_operation = *qi::space >> -(*(label[boost::bind(&(CodeGrammar::endLabelName), this)] >> *qi::space)) 
                						>> -operation[boost::bind(&(CodeGrammar::operationExists), this)] 
                						>> *qi::space >> -comment;
		        operation = zero_operation | one_operation | two_operation;

                var %= qi::char_("_a-zA-Z") >> *qi::char_("_a-zA-Z0-9");
		        array %= qi::char_("_a-zA-Z") >> *qi::char_("_a-zA-Z0-9");
                array_element %=  array >> *qi::space >> qi::char_('[') >> *qi::space >> qi::uint_ >> *qi::space >> qi::char_(']');
                wr_operand %= array_element | var;
                cast %= qi::char_('(') >> *qi::space >> -(qi::string("const") >> +qi::space) >> 
                    (
                        qi::string("uchar") |
                        qi::string("schar") |
                        qi::string("mod8") |
                        qi::string("ushort") |
                        qi::string("sshort") |
                        qi::string("mod16") |
                        qi::string("uint") |
                        qi::string("sint") |
                        qi::string("mod32")
                    ) >> *qi::space >> qi::char_(")");
                rd_operand %= -(cast >> *qi::space) >> (wr_operand | qi::int_);
                label_operand %= qi::char_("_a-zA-Z")[boost::bind(&(CodeGrammar::addLabelOperandChar), this, _1)] >>
                		*qi::char_("_a-zA-Z0-9")[boost::bind(&(CodeGrammar::addLabelOperandChar), this, _1)];
                zero_operation %= (qi::string("ret") | qi::string("nop"))[boost::bind(&(CodeGrammar::setOperation), this, _1)];
                jump_operation %=    (
                                            qi::string("jmp") |
                                            qi::string("jl") |
                                            qi::string("jg") |
                                            qi::string("call") |
                                            qi::string("je") |
                                            qi::string("jne")
                                     )[boost::bind(&(CodeGrammar::setOperation), this, _1)] 
                                     >> +qi::space >> label_operand[boost::bind(&(CodeGrammar::saveJumpOperation), this)];
                aout_operation %= qi::string("aout")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> +qi::space >> array;
                one_operation %= jump_operation | aout_operation;
                two_operation %= arith_operation | cmp_operation;
                arith_operation %=  (
                                            qi::string("mov") |
                                            qi::string("add") |
                                            qi::string("sub") |
                                            qi::string("mul") |
                                            qi::string("div") | 
                                            qi::string("mod")
                                    )[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> +qi::space >> wr_operand >> *qi::space >> qi::char_(',') >> *qi::space >> rd_operand;
                cmp_operation %= qi::string("cmp")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> 
                					+qi::space >> rd_operand >> *qi::space >> qi::char_(',') >> *qi::space >> rd_operand; 
                label %= qi::char_("_a-zA-Z")[boost::bind(&(CodeGrammar::addLabelChar), this, _1)]
                	>> *qi::char_("_a-zA-Z0-9")[boost::bind(&(CodeGrammar::addLabelChar), this, _1)] >> qi::char_(':');
                comment = qi::char_(';') >> *qi::char_;
			}

/**
Осуществляет очистку объекта от временных значений. Нужно выполнять после кажной операции трансляции.
*/
			void clear()
			{
				if(m_pcommand)
					m_pcommand -> setOperationType(Command::NONE);

				if(m_plbls)
					m_plbls -> clear();

				m_lbl = "";
				m_labelOperand = "";

				m_opExists = false;
			}



/**
Устанавливает указатель на объект Command, в который будет сохранена информация о команде.
@param pcomm - указатель на объект для сохранения информации при трансляции команды
*/
			void setCommandPtr(Command *pcomm)
			{
				m_pcommand = pcomm;
			}

/**
Устанавливает указатель на список, в который будут записываться имена меток, которыми помечена команда.
@param plabelNames - указатель на список имён меток
*/
			void setLabelNamesListPtr(std::list<std::string> *plabelNames)
			{
				m_plbls = plabelNames;
			}


/**
Указывает, было ли в оттранслированной строке определение операции.
@return true - определение было, false - не было
*/
			bool isOperationDefinition() const
			{
				return m_opExists;
			}

/**
Указывает, была ли в оттранслированной строке метка.
@return true - метка была false - не было
*/
			bool labelExists() const
			{
				return m_labelExists;
			}



		private:

/**
Оформляет операцию пререхода и вызова модуля.
*/

			void saveJumpOperation()
			{
				if(m_pcommand)
					m_pcommand -> setFirstOperand(boost::shared_ptr<Operand>(new LabelOperand(m_labelOperand)));
			}


/**
Добавляет символ к имени метке - операнду. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/
			void addLabelOperandChar(char ch)
			{
				m_labelOperand += ch;
			}


/**
Устанавливает тип операции. Используется как семантическое действие грамматики.
@param str - строковое представление типа
*/
			void setOperation(std::string str)
			{
				if(m_pcommand)
					m_pcommand -> setOperationType(Command::strToOperation(str));
			}
/**
Добавляет символ к метке. Используется как семантическое действие грамматики.
@param ch - добавляемый к имени метки символ
*/
			void addLabelChar(char ch)
			{
				m_lbl += ch;
			}

/**
Указывает на конец имени метки. Используется как семантическое действие грамматики.
*/
			void endLabelName()
			{
				m_plbls -> push_back(m_lbl);
				m_lbl = "";
			}

/**
Вызов означает встречу операции при трансляции. Используется как семантическое действие грамматики.
*/
			void operationExists()
			{
				m_opExists = true;
			}



			qi::rule<Iterator> expression, command, var, array, operation, array_element, rd_operand, wr_operand, cast, zero_operation, label_operand,
            one_operation, jump_operation, aout_operation, arith_operation, cmp_operation, two_operation, label, comment, expression_operation;

            Command									*m_pcommand;
            std::list<std::string>					*m_plbls;
            bool									m_opExists;
            bool									m_labelExists;
			std::string								m_lbl;
			std::string								m_labelOperand;
	};


		Command 								m_command;
		DataKeeper 								*m_pdata;
		CodeGrammar<std::string::iterator> 		m_grammar;
		std::list<std::string>					m_lbls;

};



#endif
