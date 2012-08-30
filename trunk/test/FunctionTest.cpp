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


/**
Проверка корректности работы со списком аргументов
*/

BOOST_AUTO_TEST_CASE(testFunction_argsList)
{
	Function func;

	std::list<std::string> lst, funcArgs;


	funcArgs = func.getArgsNames();

	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());

	lst.push_front("name1");
	func.addArgName("name1");

	funcArgs = func.getArgsNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());

	lst.push_back("name2");
	func.addArgName("name2");
	lst.push_back("name3");
	func.addArgName("name3");

	funcArgs = func.getArgsNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());

	lst.clear();
	funcArgs.clear();

	lst.push_back("name_1");
	lst.push_back("name_2");
	lst.push_back("name_3");

	funcArgs.push_back("name_1");
	funcArgs.push_back("name_2");
	funcArgs.push_back("name_3");
	func.setArgsNamesFromList(funcArgs);
	funcArgs.clear();

	funcArgs = func.getArgsNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());
}

BOOST_AUTO_TEST_SUITE_END();



/**
Тест argIsRef()
*/


BOOST_AUTO_TEST_CASE(testFunction_argsList)
{
	Function func;

	std::list<std::string> funcArgs;


	funcArgs.push_back("name_1");
	funcArgs.push_back("name_2");
	funcArgs.push_back("name_3");
	func.setArgsNamesFromList(funcArgs);
	func.addArgName("name_4");

	BOOST_CHECK_EQUAL(func.argIsRef("name_1"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_2"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_3"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_4"), false);

	func.argIsRef("name_1", true);
	func.argIsRef("name_2", true);
	func.argIsRef("name_3", true);
	func.argIsRef("name_4", true);

	BOOST_CHECK_EQUAL(func.argIsRef("name_1"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_2"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_3"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_4"), true);

	func.argIsRef("name_1", false);
	func.argIsRef("name_4", false);

	BOOST_CHECK_EQUAL(func.argIsRef("name_1"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_2"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_3"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_4"), false);


	BOOST_CHECK_THROW(func.argIsRef("name_5"), std::runtime_error);


}
