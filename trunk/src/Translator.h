#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

/**
@file Translator.h
*/


#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>


#include "VarTranslator.h"
#include "CodeBlockTranslator.h"


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
@str - проверяемая строка
@return строка содержит информацию?
*/
		static bool isEmptyOrComment(const std::string &str);

/**
Транслирование функции. Вызывается из translate().
@param header - строка - заголовок функции
*/
		void translateFunction(const std::string &);


		std::string		m_inputFileName;
		std::ifstream	m_in;
};



#endif
