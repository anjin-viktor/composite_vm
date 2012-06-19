#ifndef VAR_TRANSLATOR_H_
#define VAR_TRANSLATOR_H_


/**
@file VarTranslator.h
*/

#include <string>
#include <utility>

#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>


#include "Value.h"
#include "ParseError.h"

namespace qi = boost::spirit::qi;





/**
@class VarTranslator
@brief Класс, транслирующий объявление переменных и констант в секции .var в мнемокоде архитектуры.
*/

class VarTranslator
{
	public:
/**
Конструктор по-умолчанию.
*/

		VarTranslator();

/**
Осуществляет транслирование строки.
@param str - транслируемая строка.
@return Пара имя-переменная.
*/
		std::pair<std::string, Value> translate(std::string str);

	private:

/**
@class VarTranslator::VarGrammar
@brief Класс реализует грамматику для секции .var.
*/
	template <typename Iterator>
	class VarGrammar: public qi::grammar<Iterator>
	{
		public:
/**
Конструктор. Определяет грамматику.
*/
    	    VarGrammar(): VarGrammar::base_type(expression)
        	{
            	expression %= *qi::space >> type >> +qi::space >> var_name >> -(+qi::space >> var_value) >> *qi::space >> -comment;
            	var_name %= qi::char_("_a-zA-Z")[boost::bind(&(VarGrammar::addCharToVarName), this, _1)] >> *qi::char_("_a-zA-Z0-9")[boost::bind(&(VarGrammar::addCharToVarName), this, _1)];

            	VarGrammar *tmp = this;

            	simple_type %= (
            		qi::string("mod8") | 
            		qi::string("uchar") | 
            		qi::string("schar")
            	) [boost::bind(&(VarGrammar::setValType), this, _1)];

            	var_value %= (qi::int_)[boost::bind(&(VarGrammar::setValue), this, _1)];

            	const_ %= qi::string("const")[boost::bind(&(VarGrammar::setNoWriteable), this)];
            	type %= -(const_ >> +qi::space) >> simple_type;
            	comment %= qi::char_(';') >> *qi::char_;
            }

/**
Установка указателя на имя обрабатываемой переменной. Все изменения имени будут происходить со значением по указателю.
@param pname - указатель на имя обрабатываемого значения.
*/
            void setNamePtr(std::string *pname)
            {
  				m_pname = pname;
            }


/**
Установка указателя переменную. В ходе синтаксического анализа значение по указателю будет изменятся.
@param pval - указатель на обрабатываемое значение.
*/
            void setValuePtr(Value *pval)
            {
  				m_pval = pval;
            }

/**
Очистка параметров переменной и её имени. Используется между разбором различных строк.
*/
            void clearValue()
            {
            	if(m_pval)
            	{
        			*m_pval = Value();
  					m_pval -> setWriteable(true);
  				}

  				if(m_pname)
	  				*m_pname= "";
            }



        private:
/**Устанавливает тип обрабатываемой переменной  из переданной cтроки. Используется как семантическое действие грамматики.
@param type - строковое представление типа.
*/
			void setValType(std::string type)
			{
				m_pval -> setReadable(false);
				m_pval -> setType(Value::strToValueType(type));
			}


/**
Устанавливает обрабатываемую парсером переменную в константу. Используется как семантическое действие грамматики.
*/
			void setNoWriteable()
			{
				m_pval -> setWriteable(false);
			}


/**
Добавляет символ к имени обрабатываемой переменной. Используется как семантическое действие грамматики.
@param ch - добавляемый символ.
*/
			void addCharToVarName(char ch)
			{
				*m_pname += ch;
			}



/**
Устанавливает числовое значение обрабатываемого объекта класса Value. Используется как семантическое действие грамматики.
@param val - значение.
*/

			void setValue(long long val)
			{
				m_pval -> setValue(val);
				m_pval -> setReadable(true);
			}

           	qi::rule<Iterator> expression, type, const_, comment, simple_type, var_value, var_name;

           	std::string		*m_pname;
           	Value   		*m_pval;
		};

		std::string 							m_valName;
		Value 									m_val;
		VarGrammar<std::string::iterator>		m_grammar;
};


#endif
