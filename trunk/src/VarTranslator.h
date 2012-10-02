#ifndef VAR_TRANSLATOR_H_
#define VAR_TRANSLATOR_H_


/**
@file VarTranslator.h
*/

#include <string>
#include <utility>
#include <sstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>


#include "Value.h"
#include "Array.h"
#include "ParseError.h"
#include "DataKeeper.h"

namespace qi = boost::spirit::qi;



#include <iostream>



/**
@class VarTranslator
@brief Класс, транслирующий объявление переменных и констант в секции .var в мнемокоде архитектуры. Осуществляет обработку построчно.
Изменяет объект класса DataKeeper. По завершению транслирования пользователь получает его.
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
@param str - транслируемая строка
*/
	 void translate(std::string str);



/**
Сброс параметров. Применяется перед началом трансляции секции новой функции.
*/
    void clear();


/**
Получение результата трансляции в виде объекта DataKeeper, хранящего в себе все обработанные определения и объявления.
*/
    DataKeeper getDataKeeper() const;


/**
Функция отвечает на вопрос, является ли разобранная строка определением массива переменных.
@return разобранная строка есть определение массива?
*/
   bool isArray() const;

/**
Функция отвечает на вопрос, является ли разобранная строка определением переменной.
@return разобранная строка есть определение переменной?
*/

   bool isVariable() const;


/**
Предназначена для получения имени переменной или массива, определённых в разобранной строке.
@return имя
*/
   std::string getName() const;

/**
Получение переменной определённой в строке, переданной фунцкии VarTranslator::translate(std::string).
@return переменная
*/

   Value getValue() const;

/**
Получение массива, пределённого в строке, переданной фунцкии VarTranslator::translate(std::string).
@return массив
*/

   Array getArray() const;

	private:

/**
@class VarTranslator::VarGrammar
@brief Класс реализует грамматику для секции .var.
*/
	template <typename Iterator>
	class VarGrammar: public qi::grammar<Iterator>
	{
    private:
/**
@brief Перечисление, задающее тип разбираемого выражения
*/
	   enum ExpressionType
     {
/**
Массив
*/
      ArrayExpr = 1,
/**
Переменная  
*/
      VariableExpr,
/**
Комментарий
*/
      CommentExpr,
/**
Пустое выражение
*/
      NoneExpr
     };
  	public:
/**
Конструктор. Определяет грамматику
*/
    	VarGrammar(): VarGrammar::base_type(expression)
      {
        m_parr = NULL;
        m_pval = NULL;
        m_pname = NULL;
        m_flagArrElementNoWriteable = false;
        m_type = NoneExpr;
        expression %= var_expression[boost::bind(&(VarGrammar::setExprType), this, VariableExpr)] | 
                      arr_expression[boost::bind(&(VarGrammar::setExprType), this, ArrayExpr)] | 
                      str_expression[boost::bind(&(VarGrammar::setExprType), this, ArrayExpr)] |
                      comment_expression[boost::bind(&(VarGrammar::setExprType), this, CommentExpr)];


        comment_expression = *qi::space >> comment;

       	var_expression %= *qi::space >> var_type >> +qi::space >> name >> -(+qi::space >> var_value) >> *qi::space >> -comment;
       	name %= qi::char_("_a-zA-Z")[boost::bind(&(VarGrammar::addCharToVarName), this, _1)]
              >> *qi::char_("_a-zA-Z0-9")[boost::bind(&(VarGrammar::addCharToVarName), this, _1)];


       	simple_type %= (
       		qi::string("uchar") | 
       		qi::string("schar") |
          qi::string("mod16") |
          qi::string("ushort") |
          qi::string("sshort") |
          qi::string("mod32") |
          qi::string("uint") |
          qi::string("sint") |
          qi::string("mod8")
       	) [boost::bind(&(VarGrammar::setValType), this, _1)];

       	var_value %= (qi::long_long)[boost::bind(&(VarGrammar::setValue), this, _1)];
//       	const_ %= qi::string("const")[boost::bind(&(VarGrammar::setNoWriteable), this)];
       	var_type %= -(qi::string("const")[boost::bind(&(VarGrammar::setNoWriteable), this)] >> +qi::space) >> simple_type;
       	comment %= qi::char_(';') >> *qi::char_;


        arr_element_type %= -(qi::string("const")[boost::bind(&(VarGrammar::setArrElementNoWriteable), this)] >> +qi::space) >> simple_type;
        str_element_type %= -(qi::string("const")[boost::bind(&(VarGrammar::setArrElementNoWriteable), this)] >> +qi::space) >> str_type;

        arr_expression %= *qi::space >> -(arr_const >> +qi::space) >> 
                        (qi::string("array") | qi::string("ARRAY")) >> +qi::space >> 
                        arr_element_type >> +qi::space >>
                        name >> +qi::space >> 
                        arr_size >> *(+qi::space >> arr_val) >>
                        *qi::space >> -comment;

        arr_size %= qi::ulong_[boost::bind(&(VarGrammar::setArrSize), this, _1)];
        arr_const %= qi::string("const")[boost::bind(&(VarGrammar::setArrConst), this)];
        arr_val %= qi::long_long[boost::bind(&(VarGrammar::addInitVal), this, _1)];

        str_expression %= (*qi::space >> -(arr_const >> +qi::space) >> 
                        (qi::string("array") | qi::string("ARRAY")) >> +qi::space >> 
                        str_element_type >> +qi::space >> 
                        str_name >> +qi::space >>
                        str_init_val >>
                        *qi::space >> -comment
                        )[boost::bind(&(VarGrammar::setStrSizeAndName), this)];

        str_type %= (
          qi::string("mod8") | 
          qi::string("uchar") | 
          qi::string("schar")
        )[boost::bind(&(VarGrammar::setValType), this, _1)];

        str_name %= qi::char_("_a-zA-Z")[boost::bind(&(VarGrammar::addCharToVarNameForStr), this, _1)] >> *qi::char_("_a-zA-Z0-9")[boost::bind(&(VarGrammar::addCharToVarNameForStr), this, _1)];
        str_init_val %= qi::char_("\"") >> *(qi::print - '\"')[boost::bind(&(VarGrammar::addInitVal), this, _1)] >> qi::char_("\"");

      }

/**
Установка указателя на имя обрабатываемой переменной. Все изменения имени будут происходить со значением по указателю.
@param pname - указатель на имя обрабатываемого значения
*/
      void setNamePtr(std::string *pname)
      {
  	    m_pname = pname;
      }


/**
Установка указателя на переменную. В ходе синтаксического анализа значение по указателю будет изменятся.
@param pval - указатель на обрабатываемое значение
*/
      void setValuePtr(Value *pval)
      {
      	m_pval = pval;
      }

/**
Установка указателя на массив. В ходе синтаксического анализа будет изменяться значение по указателю.
@param parr - указатель на массив
*/

      void setArrayPtr(Array *parr)
      {
        m_parr = parr;
      }

/**
Очистка параметров массива, переменной и имени. Используется между разбором различных строк.
*/
      void clear()
      {
       	if(m_pval)
       	{
    		  *m_pval = Value();
  	    	m_pval -> setWriteable(true);
  	    }
		    if(m_pname)
          *m_pname= "";
        if(m_parr)
          *m_parr = Array();
        m_lstVal.clear();
        m_tmpStr = "";
        m_flagArrElementNoWriteable = false;
      }


/**
Указывает является ли обработанная строка определением переменной.
@return В ходе анализа разобрано определение переменной?
*/
      bool isVariableDefinition() const
      {
        return m_type == VariableExpr;
      }


/**
Указывает является ли обработанная строка определением массива переменных.
@return В ходе анализа разобрано определение массива?
*/
      bool isArrayDefinition() const
      {
        return m_type == ArrayExpr;
      }


    private:
/**Устанавливает тип обрабатываемой переменной  из переданной cтроки. Используется как семантическое действие грамматики.
@param type - строковое представление типа
*/
    	void setValType(std::string type)
			{
        if(m_pval)
			  {
      	  m_pval -> setReadable(false);
				  m_pval -> setType(Value::strToValueType(type));
        }
        if(m_parr)
          m_parr -> setType(Value::strToValueType(type));
      }


/**
Устанавливает обрабатываемую парсером переменную в константу. Используется как семантическое действие грамматики.
*/
			void setNoWriteable()
			{
        if(m_pval)
	   			m_pval -> setWriteable(false);
    	}



/**
Устанавливает в константу элемент массива. Используется как семантическое действие грамматики.
*/
      void setArrElementNoWriteable()
      {
        m_flagArrElementNoWriteable = true;
      }

/**
Добавляет символ к имени обрабатываемой переменной. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/

			void addCharToVarName(char ch)
			{
				*m_pname += ch;
			}


/**
Добавляет символ к имени обрабатываемого массива, хранящемся во временной переменной. Используется как семантическое действие грамматики.
@param ch - добавляемый символ
*/

      void addCharToVarNameForStr(char ch)
      {
        m_tmpStr += ch;
      }



/**
Устанавливает числовое значение обрабатываемого объекта класса Value. Используется как семантическое действие грамматики.
@param val - значение
*/

			void setValue(long long val)
			{
        if(m_pval)
        {
          if(Value::isOverflow(val, m_pval -> getType()))
          {
            std::stringstream ss;
            ss << "variable `" << *m_pname << "` can't fit value " << val;
            throw ParseError(ss.str());
          }
				  m_pval -> setValue(val);
				  m_pval -> setReadable(true);
			  }
      }

/**
Устанавливает размер обрабатываемого в ходе анализа массива. Используется как семантическое действие грамматики.
@param size - устанавливаемый размер
*/
      void setArrSize(std::size_t size)
      {
        if(m_parr)
          m_parr -> resize(size);

      }
/**
Устанавливает обрабатываемый парсеровм массив в константу. Используется как семантическое действие грамматики.
*/

      void setArrConst()
      {
        if(m_parr)
          m_parr -> setWriteable(false);
      }


/**
Устанавливает тип разбираемого выражения (переменная или массив). В случае если тип - массив осуществляется вызов функции для
инициализации его значений. Используется как семантическое действие грамматики. 
@param type - устанавливаемый тип
*/
      void setExprType(VarGrammar::ExpressionType type)
      {
        m_type = type;
        if(type == VarGrammar::ArrayExpr)
          setArrayValues();
      }

/**
Добавляет значение к набору для инициализации массива. Используется как семантическое действие грамматики.
@param val - значение инициализации
*/
      void addInitVal(long long val)
      {
        m_lstVal.push_back(val);
      }

/**
Осуществляет инициализацию массива набором накопленных значения.
*/
      void setArrayValues() 
      {
        if(m_lstVal.size())
        {
          std::list<long long>::iterator itr = m_lstVal.begin();

          for(int i=0; i<m_parr->size(); i++)
          {
            m_parr -> operator[](i).setValue(*itr);

            itr++;
            if(itr == m_lstVal.end())
              itr = m_lstVal.begin();
          }
          m_parr -> setReadableAll(true);
        }

        if(m_flagArrElementNoWriteable)
          m_parr -> setWriteableAll(false);
        else
          m_parr -> setWriteableAll(true);
      }

/**
Устанавливает размер массива-строки исходя из размера строки инициализации, и имя массива-строки. Используется как семантическое действие грамматики.
*/
      void setStrSizeAndName()
      {
        m_parr -> resize(m_lstVal.size());
        *m_pname = m_tmpStr;
      }


     	qi::rule<Iterator> expression, var_expression, arr_expression, str_expression, comment_expression, var_type, 
        const_, comment, simple_type, var_value, name, var, size, arr_const, arr_size, arr_val, str_type, str_init_val,
        str_name, arr_element_type, str_element_type;

      ExpressionType        m_type;
      std::string           m_tmpStr;
     	std::string		        *m_pname;
     	Value   		          *m_pval;
      Array                 *m_parr;
      std::list<long long>  m_lstVal;
      bool                  m_flagArrElementNoWriteable;
		};

		std::string 						            m_name;
    Array                               m_arr;
		Value 								            	m_val;
		VarGrammar<std::string::iterator>		m_grammar;
    DataKeeper                          m_data;
};


#endif
