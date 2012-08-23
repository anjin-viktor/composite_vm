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
#include "DataKeeper.h"


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
Получение массива команд функции.
@return массив команд функции
*/
		std::vector<Command> getCommands() const;


/**
Очистка параметров.
*/
		void clear();

/**
Установка набора данных.
@param data устанавливаемый набор
*/
		void setDataKeeper(const DataKeeper &data);


/**
Получение константной ссылки на набор данных.
@return константная ссылка на набор данных функции
*/
		const DataKeeper &getDataKeeper() const;


/**
Получение ссылки на набор данных.
@return ссылка на набор данных функции
*/
		DataKeeper &getDataKeeper();


/**
Получение указателя на набор данных.
@return указатель на набор данных функции
*/
		DataKeeper *getDataKeeperPtr();


	private:
		std::string 										m_name;
		std::map<Exception::Type, std::vector<Command> >	m_handlers;
		std::vector<Command>								m_code;
		DataKeeper											m_data;
};


#endif
