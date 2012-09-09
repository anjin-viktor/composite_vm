#ifndef CODE_EXECUTER_H_
#define CODE_EXECUTER_H_



/**
@file CodeExecuter.h
*/

#include <stack>
#include <stdexcept>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "Program.h"
#include "Context.h"
#include "Function.h"
#include "Exception.h"
#include "ArrayOperand.h"
#include "VarOperand.h"
#include "LabelOperand.h"

/**
@class CodeExecuter
@brief Класс для выполнения оттранслорованного кода программы. Предполагается что уже проверены типа операндов.
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
Установка выходного потока. Поток используется для вывода командой aout
@param stream - устанавливаемый поток
*/
		void setOutputStream(std::ostream &stream);

/**
Установка выходного потока для сообщений об ошибках
@param stream - устанавливаемый поток.
*/
		void setErrorStream(std::ostream &stream);

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


/**
Обработчик исключительных ситуаций по-умолчанию.
@param except - обрабатываемое исключение
*/
		void defaultExceptionHandler(Exception::Type except);

		std::stack<Context>		m_contexts;
		std::ostream 			*m_poutput;
		std::ostream 			*m_perrStream;
};



#endif
