#ifndef CODE_TRANSLATOR_H_
#define CODE_TRANSLATOR_H_



/**
@file CodeTranslator.h
*/


#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/bind.hpp>

#include "Value.h"
#include "Command.h"
#include "DataKeeper.h"



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
Установка набора данных, полученных при трансляции секции .var.
@param keeper - устанавливаемый набор
*/
		void setDataKeeper(const DataKeeper &keeper);

/**
Получение набора данных. Используется после трансляции, в ходе которой он будет расширен константами в коде.
@return объект, хранящий данные функции
*/
		DataKeeper getDataKeeper() const;

/**
Трансляция строки.
@param str - транслируемая строка
*/
		void translate(const std::string &str);


	private:



		DataKeeper 	m_data;
};



#endif
