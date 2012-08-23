#include <vector>

#include <boost/test/unit_test.hpp>

#include "../src/Function.h"
#include "../src/Command.h"


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


/**
Проверка методов set/getCommands
*/

BOOST_AUTO_TEST_CASE(testFunction_setGetCommands)
{
	Function func;
	std::vector<Command> code;

	Command c(Command::NOP);
	c.setLineNumber(1);

	code.push_back(c);
	c.setLineNumber(123);
	code.push_back(c);


	func.setCommands(code);
	std::vector<Command> vect = func.getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(vect.begin(), vect.end(), code.begin(), code.end());
}


BOOST_AUTO_TEST_SUITE_END();