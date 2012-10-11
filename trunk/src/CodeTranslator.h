#ifndef CODE_TRANSLATOR_H_
#define CODE_TRANSLATOR_H_



/**
@file CodeTranslator.h
*/


#include <string>
#include <list>
#include <sstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>

#include "Value.h"
#include "Command.h"
#include "DataKeeper.h"
#include "LabelOperand.h"
#include "VarOperand.h"
#include "ArrayOperand.h"
#include "CallOperand.h"




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
				m_pdata = NULL;
				m_castType = Value::NO_TYPE;
				m_operandIsConst = false;
				m_operandIsNumber = false;
				m_varIsArrayElement = false;

                expression_operation = *qi::space >> -(*(label[boost::bind(&(CodeGrammar::endLabelName), this)] >> *qi::space)) 
                						>> -operation[boost::bind(&(CodeGrammar::operationExists), this)] 
                						>> *qi::space >> -comment;
		        operation = zero_operation | one_operation | two_operation | call_operation | gtel_operation | stel_operation;

                var %= qi::char_("_a-zA-Z")[boost::bind(&(CodeGrammar::addVarNameChar), this, _1)]
                	>> *qi::char_("_a-zA-Z0-9")[boost::bind(&(CodeGrammar::addVarNameChar), this, _1)];
		        array %= qi::char_("_a-zA-Z")[boost::bind(&(CodeGrammar::addArrayNameChar), this, _1)]
		        		 >> *qi::char_("_a-zA-Z0-9")[boost::bind(&(CodeGrammar::addArrayNameChar), this, _1)];
                array_element %=  array >> *qi::space >> qi::char_('[') >> *qi::space 
                					>> qi::uint_[boost::bind(&(CodeGrammar::setArrayElementIndex), this, _1)] >> *qi::space >> qi::char_(']');
                wr_operand %= (array_element[boost::bind(&(CodeGrammar::saveArrayElementOperand), this)] |
                					var[boost::bind(&(CodeGrammar::saveVarOperand), this)]
                				);
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
                    )[boost::bind(&(CodeGrammar::setOperandType), this, _1)] >> *qi::space >> qi::char_(")");
                rd_operand %= (-(cast >> *qi::space) >> (wr_operand |
                				 qi::long_long[boost::bind(&(CodeGrammar::setNubmerOperand), this, _1)])
                			  );

                label_operand %= qi::char_("_a-zA-Z")[boost::bind(&(CodeGrammar::addLabelOperandChar), this, _1)] >>
                		*qi::char_("_a-zA-Z0-9")[boost::bind(&(CodeGrammar::addLabelOperandChar), this, _1)];
                zero_operation %= (qi::string("ret") | qi::string("nop"))[boost::bind(&(CodeGrammar::setOperation), this, _1)];
                jump_operation %=    (
                                            qi::string("jmp") |
                                            qi::string("jl") |
                                            qi::string("jg") |
                                            qi::string("je") |
                                            qi::string("jne")
                                     )[boost::bind(&(CodeGrammar::setOperation), this, _1)] 
                                     >> +qi::space >> label_operand[boost::bind(&(CodeGrammar::saveJumpOperation), this)];
                aout_operation %= qi::string("aout")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> +qi::space >> 
                				array[boost::bind(&(CodeGrammar::saveArrayOperand), this)];
                one_operation %= jump_operation | aout_operation;
                two_operation %= arith_operation | cmp_operation | arr_rsz_operation | arr_size_operation;
                arith_operation %=  (
                                            qi::string("mov")   |
                                            qi::string("add")   |
                                            qi::string("sub")   |
                                            qi::string("mul")   |
                                            qi::string("div")   | 
                                            qi::string("mod")   |
                                            qi::string("shrwo") |
                                            qi::string("shlwo") |
                                            qi::string("shr")   |
                                            qi::string("shl")   |
                                            qi::string("ror")   |
                                            qi::string("rol") 
                                    )[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> +qi::space >> 
                                    wr_operand[boost::bind(&(CodeGrammar::saveFirstOperand), this)] >> 
                                    *qi::space >> qi::char_(',') >> *qi::space >> rd_operand[boost::bind(&(CodeGrammar::saveSecondOperand), this)];
                arr_rsz_operation %= qi::string("rsz")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> +qi::space 
                					 >> array[boost::bind(&(CodeGrammar::saveArrayOperand), this)] >>  *qi::space >> 
                					 qi::char_(',') >> *qi::space >>  rd_operand[boost::bind(&(CodeGrammar::saveSecondOperand), this)];

                arr_size_operation %= qi::string("size")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> +qi::space
                					 >> wr_operand[boost::bind(&(CodeGrammar::saveFirstOperand), this)] >> *qi::space >> 
                					 qi::char_(',') >> *qi::space >> array[boost::bind(&(CodeGrammar::saveArraySecondOperand), this)];
                cmp_operation %= qi::string("cmp")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >> 
                					+qi::space >> rd_operand[boost::bind(&(CodeGrammar::saveFirstOperand), this)] >>
                					 *qi::space >> qi::char_(',') >> *qi::space >> rd_operand[boost::bind(&(CodeGrammar::saveSecondOperand), this)]; 
                gtel_operation %= qi::string("gtel")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >>
                					+qi::space >> wr_operand[boost::bind(&(CodeGrammar::saveFirstOperand), this)]
                					>> *qi::space >> qi::char_(',') >> *qi::space
                					>> array[boost::bind(&(CodeGrammar::saveArraySecondOperand), this)] 
                					>>  *qi::space >> qi::char_(',') >> *qi::space
                					>> rd_operand[boost::bind(&(CodeGrammar::saveThirdOperand), this)];

                stel_operation %= qi::string("stel")[boost::bind(&(CodeGrammar::setOperation), this, _1)] >>
                					+qi::space >> rd_operand[boost::bind(&(CodeGrammar::saveFirstOperand), this)]
                					>> *qi::space >> qi::char_(',') >> *qi::space
                					>> array[boost::bind(&(CodeGrammar::saveArraySecondOperand), this)] 
                					>>  *qi::space >> qi::char_(',') >> *qi::space
                					>> rd_operand[boost::bind(&(CodeGrammar::saveThirdOperand), this)];


                label %= qi::char_("_a-zA-Z")[boost::bind(&(CodeGrammar::addLabelChar), this, _1)]
                	>> *qi::char_("_a-zA-Z0-9")[boost::bind(&(CodeGrammar::addLabelChar), this, _1)] >> qi::char_(':');
                comment = qi::char_(';') >> *qi::char_;
			
				call_operation %= qi::string("call") [boost::bind(&(CodeGrammar::setOperation), this, _1)]
					>> +qi::space >> label_operand[boost::bind(&(CodeGrammar::saveJumpOperation), this)] 
					>> -(*(call_operand[boost::bind(&(CodeGrammar::addCallOperand), this)]));
				call_operand %= *qi::space >> ',' >> *qi::space >> (call_wr_operand | rd_operand
					[boost::bind(&(CodeGrammar::setCallOperandToConst), this)]); 
				call_wr_operand = (array_element[boost::bind(&(CodeGrammar::saveCallArrayElementOperand), this)] |
                					var[boost::bind(&(CodeGrammar::saveVarOrArrOperand), this)]);
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
				m_arrName = "";
				m_varName = "";

				m_callOpNo = 1;
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

/**
Устанавливает указатель на набор данных. Набор данных потребуется при трансляции команд с переменными.
@param pdata - указатель на набор данных
*/
			void setDataKeeperPtr(DataKeeper *pdata)
			{
				m_pdata = pdata;
			}

		private:

/**
Оформляет рассматриваемый массив как первый операнд функции
*/

			void saveArrayOperand()
			{
				ArrayOperand * op = new ArrayOperand();

				if(m_pdata)
					if(m_pdata -> isArray(m_arrName) == false)
						throw ParseError("array with name `" + m_arrName + "` not exists");
					else
						op -> setArrayPtr(&m_pdata -> getArray(m_arrName));

				if(m_pcommand)
					m_pcommand -> setFirstOperand(boost::shared_ptr<Operand>(op));
			}



/**
Оформляет рассматриваемый массив как второй операнд функции
*/

			void saveArraySecondOperand()
			{
				ArrayOperand * op = new ArrayOperand();

				if(m_pdata)
					if(m_pdata -> isArray(m_arrName) == false)
						throw ParseError("array with name `" + m_arrName + "` not exists");
					else
						op -> setArrayPtr(&m_pdata -> getArray(m_arrName));

				if(m_pcommand)
					m_pcommand -> setSecondOperand(boost::shared_ptr<Operand>(op));
			}





/**
Установливает числовую константу - операнд операции. Используется как семантическое действие грамматики.
@param n - устанавливаемое значение
*/

			void setNubmerOperand(long long n)
			{
				std::stringstream ss;
				ss << n;
				std::string valName = ss.str();
				Value val;

				if(m_pdata)
				{
					if(m_pdata -> isExists(valName) == false)
					{
						val = Value(n, Value::NO_TYPE, true, false);
						m_pdata -> addVar(val, valName);
					}

					m_currentVar = VarOperand(&(m_pdata -> getVarValue(valName)));
					m_callOp.setValuePtr(&(m_pdata -> getVarValue(valName)));
				}
				m_currentVar.setConstancy(true);
				m_operandIsNumber = true;
			

				m_callOpIsVal = true;
			}



/**
Устанавливает тип, к которому приводится переменная.
@param str - строковое представление типа
*/
			void setOperandType(std::string str)
			{
				m_castType = Value::strToValueType(str);
				m_operandIsConst = true;
			}


/**
Сохраняет текущий строимый операнд как первый операнд. Используется как семантическое действие грамматики.
*/
			void saveFirstOperand()
			{
				m_currentVar.setType(m_castType);

				if(m_operandIsNumber == false)
					m_currentVar.setConstancy(m_operandIsConst);

				if(m_pcommand)
					m_pcommand -> setFirstOperand(boost::shared_ptr<Operand>(new VarOperand(m_currentVar)));

				m_castType = Value::NO_TYPE;
				m_operandIsConst = false;
				m_operandIsNumber = false;
			}


/**
Сохраняет текущий строимый операнд как второй операнд. Используется как семантическое действие грамматики.
*/
			void saveSecondOperand()
			{
				m_currentVar.setType(m_castType);

				if(m_operandIsNumber == false)
					m_currentVar.setConstancy(m_operandIsConst);


				if(m_pcommand)
					m_pcommand -> setSecondOperand(boost::shared_ptr<Operand>(new VarOperand(m_currentVar)));

				m_operandIsNumber = false;
				m_castType = Value::NO_TYPE;
				m_operandIsConst = false;

			}



/**
Сохраняет текущий строимый операнд как третий операнд. Используется как семантическое действие грамматики.
*/
			void saveThirdOperand()
			{
				m_currentVar.setType(m_castType);

				if(m_operandIsNumber == false)
					m_currentVar.setConstancy(m_operandIsConst);


				if(m_pcommand)
					m_pcommand -> setOperand(2, boost::shared_ptr<Operand>(new VarOperand(m_currentVar)));

				m_operandIsNumber = false;
				m_castType = Value::NO_TYPE;
				m_operandIsConst = false;

			}


/**
Оформляет элемент массива как операнд. Используется как семантическое действие грамматики.
*/

			void saveArrayElementOperand()
			{
				if(m_pdata)
				{
					if(m_pdata -> isArray(m_arrName) == false)
						throw ParseError("array with name `" + m_arrName + "` not exists");

					m_currentVar = VarOperand(&(m_pdata -> getArray(m_arrName)), m_arrElementIndx, Value::NO_TYPE);
				}
				m_varName = "";
				m_arrName = "";
			}


/**
Оформляет переменную как операнд. Используется как семантическое действие грамматики.
*/
			void saveVarOperand()
			{
				if(m_pdata)
				{
					if(m_pdata -> isVar(m_varName) == false)
						throw ParseError("variable with name `" + m_varName + "` not exists");
			
					m_currentVar = VarOperand(&(m_pdata -> getVarValue(m_varName)));

					m_callOp.setValuePtr(&(m_pdata -> getVarValue(m_varName)));
					m_callOpIsVal = true;

				}
				m_varName = "";
				m_arrName = "";

			}



/**
Добавляет символ к имени переменной. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/

			void addVarNameChar(char ch)
			{
				m_varName += ch;
			}


/**
Добавляет символ к имени массива. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/
			void addArrayNameChar(char ch)
			{
				m_arrName += ch;
			}



/**
Устанавливает индекс смещения в массиве
@param n - смещение
*/

			void setArrayElementIndex(int n)
			{
				m_arrElementIndx = n;
			}
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


/**
Сохранение операнда команды call(это переменная или массив). Используется как семантическое действие грамматики.
*/
			void saveVarOrArrOperand()
			{
				if(m_pdata)
				{
					if(m_pdata -> isVar(m_varName))
					{
						m_callOp.setValuePtr(&(m_pdata -> getVarValue(m_varName)));
						m_pdata -> getVarValue(m_varName).canBeInit();
						m_callOpIsVal = true;

					}
					else if(m_pdata -> isArray(m_varName))
					{
						m_callOp.setArrayPtr(&(m_pdata -> getArray(m_varName)));
						m_callOpIsVal = false;
					}
					else 
						throw ParseError("variable with name `" + m_varName + "` not exists");
				}				
				m_varName = "";
				m_arrName = "";
			}


/**
Сохранение операнда - элемента массива для команды call. Используется как семантическое действие грамматики.
*/
			void saveCallArrayElementOperand()
			{
				if(m_pdata)
				{
					if(m_pdata -> isArray(m_arrName) == false)
						throw ParseError("array with name `" + m_arrName + "` not exists");

					m_callOpIsVal = true;
					m_varIsArrayElement = true;

					m_callOp.setArrayElement(&(m_pdata -> getArray(m_arrName)), m_arrElementIndx);

				}
			}

/**
Добавление операнда команды call. Используется как семантическое действие грамматики.
*/
			void addCallOperand()
			{
				boost::shared_ptr<CallOperand> op(new CallOperand);


				if(m_callOpIsVal)
				{
					if(m_castType != Value::NO_TYPE)
					{
						m_callOp.setValueType(m_castType);
						m_callOp.setWriteable(false);
					}

					if(m_operandIsConst || m_operandIsNumber)
						m_callOp.setWriteable(false);

				}
				else 
				{
					if(m_operandIsConst)
						m_callOp.setWriteable(false);
				}


				*op = m_callOp;

				m_pcommand -> setOperand(m_callOpNo, boost::shared_ptr<Operand>(op));
				m_callOpNo++;
				m_castType = Value::NO_TYPE;
				m_operandIsConst = false;
				m_callOp.setWriteable(true);
				m_varIsArrayElement = false;
				m_arrName = "";
			}



/**
Установка в константу строящегося операдна команды call. Используется как семантическое действие грамматики
*/

			void setCallOperandToConst()
			{
				m_operandIsConst = true;
			}

			qi::rule<Iterator> expression, command, var, array, operation, array_element, rd_operand, wr_operand, cast, zero_operation, label_operand,
            one_operation, jump_operation, aout_operation, arith_operation, cmp_operation, two_operation, label, comment, expression_operation, arr_rsz_operation, 
            call_operand, call_operation, call_wr_operand, gtel_operation, stel_operation, arr_size_operation;

            Command									*m_pcommand;
            std::list<std::string>					*m_plbls;
			DataKeeper	 							*m_pdata;
            bool									m_opExists;
            bool									m_labelExists;
			std::string								m_lbl;
			std::string								m_labelOperand;
			std::string								m_varName;
			std::string								m_arrName;
			std::size_t								m_arrElementIndx;
			VarOperand 								m_currentVar;
			Value::ValueType 						m_castType;
			bool									m_operandIsConst;
			bool									m_operandIsNumber;
			bool									m_callOpIsVal;
			std::size_t								m_callOpNo;
			bool									m_varIsArrayElement;
			CallOperand 							m_callOp;
	};


/**
Осуществляет проверка команды на корректность. Проверяются операнды, их права доступа, типы. Вслучае ошибки герерируется исключение
@throw ParseError - в случае некорректной команды.
*/

		void checkCorrectness() const throw(ParseError);


		Command 								m_command;
		DataKeeper 								*m_pdata;
		CodeGrammar<std::string::iterator> 		m_grammar;
		std::list<std::string>					m_lbls;

};



#endif
