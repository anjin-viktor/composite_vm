#ifndef FUNCTION_H_
#define FUNCTION_H_



/**
@file Function.h
*/

#include <map>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>

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


/**
Получение списка аргументов в порядке слева на право
@return список аргументов
*/
		std::list<std::string> getArgsNames() const;


/**
Добавление имени переменной к списку аргументов. Помещать следует слева на право.
@param str - добавляемое имя
*/
		void addArgName(const std::string &str);

/**
Установка списка имен аргументов. В списке имена следует перечислять слева на право в порядке их 
нахождения в сигнатуре.
@param lst - устанавливаемый список
*/
		void setArgsNamesFromList(const std::list<std::string> &lst);

/**
Указание, является ли аргумент ссылкой.
@param str - имя аргумента
@param f - true - ссылка, иначе - false
*/
		void argIsRef(const std::string &name, bool f);


/**
Яляется ли аргумент ссылкой?
@param str - имя аргумента
@return true - ссылка, иначе - false
*/
		bool argIsRef(const std::string &name) const;


	private:
		std::string 										m_name;
		std::map<Exception::Type, std::vector<Command> >	m_handlers;
		std::vector<Command>								m_code;
		DataKeeper											m_data;
/*
аргументы функции в порядке слева на право
*/
		std::list<std::string>								m_args;
		std::map<std::string, bool>							m_argsIsRefs;
};


#endif
