#ifndef EXCEPTION_H_
#define EXCEPTION_H_

/**
@file Exception.h
*/

#include <string>


namespace Exception
{
/**
@brief перечисление, предназначеное для представления типа исключительной ситуации.
*/
	enum Type
	{
		NumericError,
		ConstraintError,
		NoType
	};

/**
Преобразование строки с именем исключения в формат Exception::Type.
@param str - строковое представление типа
@return представление типа в виде Exception::Type (если строка некорректна Exception::NoType)
*/
	Type strToExceptionType(const std::string &str);


/**
Преобрабование Exception::Type в std::string
@param except - представление типа исключения в виде Exception::Type
@return строковое представление типа ислючения
*/
	std::string exceptionTypeToStr(Exception::Type except);

};



#endif
