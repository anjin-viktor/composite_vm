#ifndef HEADER_TRANSLATOR_H_
#define HEADER_TRANSLATOR_H_



/**
@file HeaderTranslator.h
*/

#include <string>
#include <stdexcept>
#include <list>

#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>


#include "DataKeeper.h"
#include "Array.h"
#include "Value.h"

/**
@class HeaderTranslator
@brief Используется для трансляции секции .name вычислительного модуля
*/



namespace qi = boost::spirit::qi;


class HeaderTranslator
{
	public:
/**
Конструктор по-умолчанию
*/
		HeaderTranslator();


/**
Деструктор
*/
		~HeaderTranslator();


/**
Установка указателя на объект с данными модуля. В него заносятся передаваемые аргументы.
@param pkeeper - указатель на объект, используемый для хранение данных транслируемого модуля
*/
		void setDataKeeperPtr(DataKeeper *pkeeper); 



/**
Трансляция заголовка.
@param str - транслируемая строка
*/
		void translate(std::string str);


/**
Получение имени функции. Подразумевается, что вход - корректная строка, являющаяся заголовком модуля.
@param str - заголовок модуля.
*/
		static std::string getNameFromStr(const std::string &str);


/**
Получение имени модуля. Используется после транслирования строки с использованием функции translate()
@return имя модуля
*/
		std::string getName() const;


/**
Очистка транслятора. Нужно вызывать перед началом трансляции.
*/
		void clear();


/**
Получение списка имён аргументов в порядке слева на право.
@return список имён аргументов модуля
*/
		std::list<std::string> getArgsNames() const;


/**
Отвечает на вопрос, является ли операнд ссылкой.
@param name - имя операнда
@return true - операнд - ссылка, иначе - false
*/
		bool argIsRef(const std::string &name) const;


	private:

/**
@class HeaderGrammar
@brief Реализует грамматику для секции .name
*/
		template <typename Iterator>
		class HeaderGrammar: public qi::grammar<Iterator>
		{
			public:
/**
Конструктор. Реализует грамматику.
*/
				HeaderGrammar(): HeaderGrammar::base_type(expression)
				{
					m_varIsConst = false;
					m_plstArgs = NULL;
					m_argIsRef = false;
					m_pargsIsRefs = NULL;
					m_pdata = NULL;
					m_arrIsConst = false;

					expression = *qi::space >> qi::string(".name") >> +qi::space >> name
								>> -(+qi::space >> param) >>  *(*qi::space >> qi::char_(',') >> *qi::space >> param)
								>> *qi::space;

					name = qi::char_("_a-zA-Z")[boost::bind(&(HeaderGrammar::addNameChar), this, _1)] 
							>> *qi::char_("_a-zA-Z0-9")[boost::bind(&(HeaderGrammar::addNameChar), this, _1)];

					var_name = qi::char_("_a-zA-Z")[boost::bind(&(HeaderGrammar::addVarNameChar), this, _1)] 
					 	>> *qi::char_("_a-zA-Z0-9")[boost::bind(&(HeaderGrammar::addVarNameChar), this, _1)] ;

					arr_name = qi::char_("_a-zA-Z")[boost::bind(&(HeaderGrammar::addArrNameChar), this, _1)]
						>> *qi::char_("_a-zA-Z0-9")[boost::bind(&(HeaderGrammar::addArrNameChar), this, _1)];

					param = *qi::space >> -(qi::char_('&')[boost::bind(&(HeaderGrammar::argIsRef), this)])
							>> *qi::space >> (array[boost::bind(&(HeaderGrammar::saveArr), this)] | 
							var[boost::bind(&(HeaderGrammar::saveVar), this)]) >> *qi::space;

					var = -(qi::string("const")[boost::bind(&(HeaderGrammar::setVarToConst), this)] >> +qi::space) >> simple_type >> ref_or_space >> var_name;
					array = -(qi::string("const")[boost::bind(&(HeaderGrammar::setArrToConst), this)] >> +qi::space) >> 
							(qi::string("array") | qi::string("ARRAY")) >> +qi::space >> simple_type >> ref_or_space
							>> arr_name;
			       	simple_type %= (
       					qi::string("mod8") | 
       					qi::string("uchar") | 
       					qi::string("schar") |
        				qi::string("mod16") |
        				qi::string("ushort") |
         				qi::string("sshort") |
        				qi::string("mod32") |
        				qi::string("uint") |
        				qi::string("sint")
       				)[boost::bind(&(HeaderGrammar::setType), this, _1)];

       				ref_or_space = (*qi::space >> -(qi::char_('&')[boost::bind(&(HeaderGrammar::argIsRef), this)])
									>> *qi::space) | +qi::space;
				}

/**
Очистка объекта.
*/
				void clear()
				{
					m_currentVarName = m_currentArrName = m_name = "";
				}


/**
Получение имени модуля.
@return имя модуля
*/
				std::string getName() const
				{
					return m_name;
				}


/**
Установка указателя на объект для записи данных.
@param pdata - устанавливаемый указатель
*/
				void setDataKeeperPtr(DataKeeper *pdata)
				{
					m_pdata = pdata;
				}

/**
Установка указателя на список для сохранения имён аргументов.
@param plst - указатель на список
*/
				void setArgsNamesListPtr(std::list<std::string> *plst)
				{
					m_plstArgs = plst;
				}

/**
Установка указателя на список для сохранения информации о том, является ли операнд ссылкой
@param plst - указатель на список
*/
				void setArgsIsRefsListPtr(std::list<bool> *plst)
				{
					m_pargsIsRefs = plst;
				}


			private:
/**
Добавление символа к имени модуля. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/
				void addNameChar(char ch)
				{
					m_name += ch;
				}

/**
Добавление символа к имени переменной. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/
				void addVarNameChar(char ch)
				{
					m_currentVarName += ch;
				}

/**
Добавление символа к имени массива. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/
				void addArrNameChar(char ch)
				{
					m_currentArrName += ch;
				}


/**
Установка типа переменной / элемента массива. Используется как семантическое действие грамматики.
@param str - строковое представление устанавливаемого типа
*/
				void setType(std::string str)
				{
					m_type = Value::strToValueType(str);
				}


/**
Вызов означает окончание разбора переменной и её сохранение
*/
				void saveVar()
				{
					if(m_pdata)
						m_pdata -> addVar(Value(0, m_type, true, !m_varIsConst), m_currentVarName);

					if(m_plstArgs)
						m_plstArgs -> push_back(m_currentVarName);

					if(m_pargsIsRefs)
						m_pargsIsRefs -> push_back(m_argIsRef);

					m_varIsConst = false;
					m_currentVarName = "";
					m_argIsRef = false;
					m_arrIsConst = false;
				}


/**
Вызов означает окончание разбора массива и его сохранение
*/
				void saveArr()
				{
					if(m_pdata)
					{
						Array arr(0, m_type);
						arr.setWriteable(!m_arrIsConst);
						m_pdata -> addArray(arr, m_currentArrName);
					}

					if(m_plstArgs)
						m_plstArgs -> push_back(m_currentArrName);

					if(m_pargsIsRefs)
						m_pargsIsRefs -> push_back(m_argIsRef);

					m_currentArrName = "";
					m_argIsRef = false;
					m_arrIsConst = false;
				}


/**
Устанавливает разбируемую переменную в константу. Используется как семантическое действие грамматики.
*/
				void setVarToConst()
				{
					m_varIsConst = true;
				}



/**
Устанавливает разбируемуый массив в константу. Используется как семантическое действие грамматики.
*/
				void setArrToConst()
				{
					m_arrIsConst = true;
				}



/**
Указывает, что строящийся аргумент является ссылкой
*/
				void argIsRef()
				{
					m_argIsRef = true;
				}


				DataKeeper 				*m_pdata;
				Value::ValueType 		m_type;
				std::string 			m_currentVarName;
				std::string				m_currentArrName;
				std::string				m_name;
				bool					m_varIsConst;
				bool					m_argIsRef;
				bool					m_arrIsConst;
				qi::rule<Iterator> 		expression, name, param, simple_type, array, var, var_name, arr_name, ref_or_space;
				std::list<std::string>	*m_plstArgs;
				std::list<bool>			*m_pargsIsRefs;
		};


		DataKeeper 								*m_pdata;
		std::string								m_name;
		HeaderGrammar<std::string::iterator>	m_grammar;
		std::list<std::string>					m_argsNames;
		std::map<std::string, bool>				m_argsIsRefs;
		std::list<bool>							m_argsIsRefsList;
};



#endif
