#include <boost/test/unit_test.hpp>

#include "../src/Function.h"



/**
@file FunctionTest.cpp
@brief Набор модульных тестов для класса Function.
*/



BOOST_AUTO_TEST_SUITE(FUNCTION_TEST_SUITE);


/**
Проверка конструктора
*/


BOOST_AUTO_TEST_CASE(testFunction)
{
	Function func;
	func.setName("name");
	BOOST_CHECK_EQUAL(func.getName(), "name");

	std::vector<Command> commands;
	func.setExceptionHandler(Exception::NumericError, commands);
	func.setCommands(commands);
}



BOOST_AUTO_TEST_SUITE_END();