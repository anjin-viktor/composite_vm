#ifndef CODE_EXECUTER_H_
#define CODE_EXECUTER_H_



/**
@file CodeExecuter.h
*/

#include <stack>
#include <stdexcept>

#include "Program.h"
#include "Context.h"
#include "Function.h"
#include "Exception.h"

/**
@class CodeExecuter
@brief Класс для выполнения оттранслорованного кода программы
*/

class CodeExecuter
{
	public:
/**
Конструктор.
*/
		CodeExecuter();
/**
Деструктор.
*/
		~CodeExecuter();




/**
Выполнение программы. Работает с оттранслированным кодом из Program
*/
		void exec();

	private:
/**
Начальная инициализация. Выполняется перед началом выполнения программы.
*/
		void init();

/**
Выполнение следующей команды
@return возникщее исключение, Exception::NoType при их отсутствии
*/
		Exception::Type exec_command();

		std::stack<Context>		m_contexts;
};



#endif
