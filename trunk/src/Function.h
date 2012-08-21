#ifndef FUNCTION_H_
#define FUNCTION_H_



/**
@file Function.h
*/

#include <map>
#include <string>
#include <vector>

#include "Value.h"
#include "Command.h"
#include "Exception.h"


/**
@class Function
@brief Предназначен для представления функции. Содержит в себе вектор команд, переменные, ссылки на внешние переменные.
*/

class Function
{
	public:
/**
Конструктор по-умолчанию.
*/
		Function();

/**
Деструктор.
*/
		~Function();

/**
Установка имени функции. Заполняется при трансляции. Используется для поиска функции при вызове.
@param name - имя функции
*/
		void setName(const std::string &name);


/**
Получение имени функции.
@return имя функции
*/
		std::string getName() const;

/**
Установка обработчика ислючительной ситуации.
@param except - тип ислючительной ситуации, для которого устанавливается обработчик
@param commands - вектор команд
*/
		void setExceptionHandler(Exception::Type except, const std::vector<Command> &commands);

/**
Установка массива команд функции.
@param commands - устанавливаемый вектор команд
*/
		void setCommands(const std::vector<Command> &commands);


/**
Очистка параметров.
*/
		void clear();


	private:
		std::string 										m_name;
		std::map<Exception::Type, std::vector<Command> >	m_handlers;
		std::vector<Command>								m_code;
};


#endif
