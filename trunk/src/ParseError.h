#ifndef PARSE_ERROR_
#define PARSE_ERROR_


/**
@file ParseError.h
*/

#include <stdexcept>
#include <string>

/**
@class ParseError 
@brief Класс, предназначенный для оповещения при происществии ошибки в ходе трансляции. Используется в качестве выкидываемого 
исключения.
*/


class ParseError: public std::exception
{
	public:
/**
Конструктор от строки - сообщения об ошибке.
@param msg - сообщение, описывающую произощедщую ошибку
*/
		ParseError(const std::string &msg);

		virtual ~ParseError() throw();

/**
Метод для получения сообщения о произошедшей ошибке.
@return сообщение о ошибке
*/

		virtual const char *what() const  throw();

/**
Функций для установки сообщения.
@param msg - устанавливаемое сообщения
*/
		void setMsg(const std::string &msg);

	private:
		std::string 	m_msg;
};


#endif
