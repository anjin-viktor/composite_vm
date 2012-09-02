#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

/**
@file Translator.h
*/


#include <fstream>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/regex.hpp>

#include "VarTranslator.h"
#include "CodeBlockTranslator.h"
#include "Program.h"
#include "HeaderTranslator.h"
#include "Operand.h"
#include "LabelOperand.h"
#include "Exception.h"

/**
@class Translator
@brief Класс, производящий трансляцию кода из конкретного файла.

*/
class Translator
{
	public:
/**
Конструктор от строки - имени файла. При трансляции будет обрабатываться код из этого файла.
@param fname - имя файла.
*/
		Translator(std::string fname = "");
/**
Деструктор.
*/
		~Translator();

/**
Установка входного файла. При трансляции будет обрабатываться код из этого файла.
@param fname - имя файла
*/
		void setInputFileName(const std::string &fname);


/**
Получение имени входного файла.
@return имя файла
*/
		std::string getInputFileName() const;

/**
Осуществляет трансляцию кода из файла, указаного в конструкторе или методом setInputFile()
*/
		void translate();

	private:
/**
Чтение строки кода из указанного файла. Результат помещается в переданную по указателю строку
@param str - строка для результата
@return достигнут конец файла?
*/
		bool readString(std::string &str);


/**
Проверка на вхождение в строку полезной нагрузки.
@param str - проверяемая строка
@return строка содержит информацию?
*/
		static bool isEmptyOrComment(const std::string &str);

/**
Транслирование функции. Вызывается из translate().
@param header - строка - заголовок функции
*/
		void translateFunction(const std::string &);


/**
Транслирование обработчика ислючительной ситуации. Вызывается из translateFunction90
@param header - строка - заголовок обработчика
@param name - имя функции, которой принадлежит обработчик
@return последняя строка(либо определение нового обработчика, либо .end)
*/
		std::string translateExceptionHandler(const std::string &header, const std::string &name);

/**
Проверка наличия в программе функций с именами - операндами команд call и проверка соответствия типов параметров
*/
		void callOperandsCheck() const;



/**
Проверка команды call. Используется из callOperandsCheck().
@param command - команда
@param funcName - имя функции, содержащей команду
*/
		void callCheck(Command command, const std::string &funcName) const;

		std::string		m_inputFileName;
		std::ifstream	m_in;
		std::size_t		m_lineNumb;
};



#endif
