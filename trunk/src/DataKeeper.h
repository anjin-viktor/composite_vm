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
@throw ParseError - в случае существования переменной или массива с указанным именем
*/
		void addVar(const Value &var, const std::string &name) throw(std::runtime_error);


/**
Добавление массива с указанным именем. Если объект с таким именем уже существует генерируется исключение.
@param arr - добавляемое значение
@param name - устанавливаемое имя
@throw ParseError - в случае существования переменной или массива с указанным именем
*/

		void addArray(const Array &arr, const std::string &name) throw(std::runtime_error);

/**
Проверка на существование переменной с указанным именем.
@param str - проверяемое имя
@return true - объект с указанным именем существует, иначе - false
*/

		bool isExists(const std::string &str) const;


/**
Проверка на существование переменной с указанным именем.
@param str - проверяемое имя
@return true - это переменная
*/
		bool isVar(const std::string &str) const;


/**
Проверка на существование массива с указанным именем.
@param str - проверяемое имя
@return true - это массив
*/
		bool isArray(const std::string &str) const;



/**
Взятие ссылки на значение переменной с указанным именем.
@param name - имя требуемой переменной
@return ссылка на искомую переменную
@throw std::runtime_error - в случае несуществования переменной с искомым именем
*/

		Value &getVarValue(const std::string &name) throw(std::runtime_error);

/**
Взятие константной ссылки на значении переменной с указанным именем.
@param name - имя требуемой переменной
@return константная ссылка на искомую переменную
@throw std::runtime_error - в случае несуществования переменной с требуемым именем
*/

		const Value &getVarValue(const std::string &name) const throw(std::runtime_error);

/**
Взятие ссылки на массив с указанным именем.
@param name - имя требуемого массива
@return ссылка на искомый массив
@throw std::runtime_error - в случае несуществования массива с требуемым именем
*/
		Array &getArray(const std::string &name) throw(std::runtime_error);

/**
Взятие константной ссылки на массив с указанным именем.
@param name - имя требуемого массива
@return константная ссылка  на искомый массив
@throw std::runtime_error - в случае несуществования массива с требуемым именем
*/
		const Array &getArray(const std::string &name) const throw(std::runtime_error);


/**
Очистка объекта.
*/
		void clear();




/**
Получение числа сохраненных в объекте массивов и переменных. Используется в первую очередь для тестирования
@return число массивов + число переменных
*/
		std::size_t getNumberOfElements() const;


	private:
		std::map<std::string, Value>		m_vars;
		std::map<std::string, Array>		m_arrs;
};







#endif
