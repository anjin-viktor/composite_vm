#ifndef DATA_KEEPER_H_
#define DATA_KEEPER_H_

/**
@file DataKeeper.h
*/



#include <map>
#include <string>

#include "Value.h"
#include "Array.h"
#include "ParseError.h"

/**
@class DataKeeper
@brief Предназначен для хранения данных функции. Данными являются переменные, массивы, используемые в командах константы, переданные аргументы.
*/

class DataKeeper
{
	public:
/**
Конструктор по-умолчанию.
*/
		DataKeeper();
/**
Деструктор.
*/
		~DataKeeper();


/**
Добавление переменной с указанным именем. Если объект с таким именем уже существует генерируется исключение.
@param var - добавляемое значение
@param name - устанавливаемое имя переменной
@throw ParseError - вслучае существования переменной или массива с указанным именем
*/
		void addVar(const Value &var, const std::string &name) throw(ParseError);


/**
Добавление массива с указанным именем. Если объект с таким именем уже существует генерируется исключение.
@param arr - добавляемое значение
@param name - устанавливаемое имя
@throw ParseError - вслучае существования переменной или массива с указанным именем
*/

		void addArray(const Array &arr, const std::string &name) throw(ParseError);

	private:
		std::map<std::string, Value>		m_vars;
		std::map<std::string, Array>		m_arrs;
};







#endif
