#ifndef PROGRAM_H_
#define PROGRAM_H_



/**
@file Program.h
*/

#include <string>
#include <map>
#include <stdexcept>
#include <list>


#include "Function.h"

/**
@class Program
@brief Служит для хранения кода оттранслированной программы.
*/

class Program
{
	public:
/**
Получение ссылки для использования функциональности
*/
		static Program& getInstance();


/**
Получение ссылки на функцию по её имени. В случае отсутствия генерируется исключение.
@param name - имя функции
*/
		Function &getFunction(const std::string &name);


/**
Получение константной ссылки на функцию по её имени. В случае отсутствия генерируется исключение.
@param name - имя функции
*/
		const Function &getFunction(const std::string &name) const;


/**
Добавление в программу функции.
@param func - добавляемая функция.
*/
		void addFunction(const Function &func);


/**
Проверка на существование функции с указанным именем
@param name - проверяемое имя
*/
		bool functionIsExists(const std::string &name) const;


/**
Получение числа функций в программе.
@return число функций
*/
		std::size_t numberOfFunctions() const;


/**
Получение списка имён функций программы
@return список имйн функций
*/
		std::list<std::string> getFunctionNames() const;



/**
Очистка программы.
*/
		void clear();


	private:
/**
Конструктор по-умолчанию. Объект - одиночка.
*/
		Program();
/**
Деструктор. Объект - одиночка.
*/
		~Program();


		std::map<std::string, Function>		m_functions;
		static Program						m_instance;
};




#endif
