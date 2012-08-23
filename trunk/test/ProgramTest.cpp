#include <stdexcept>

#include <boost/test/unit_test.hpp>
#include "../src/Program.h"



/**
@file ProgramTest.cpp
@brief Набор модульных тестов для класса Program
*/


BOOST_AUTO_TEST_SUITE(Program_TEST_SUITE);



BOOST_AUTO_TEST_CASE(ProgramTest_1)
{
	Program::getInstance().clear();
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("test_function"), false);

	Function func;
	func.setName("test_function");
	Program::getInstance().addFunction(func);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("test_function"), true);	

	BOOST_CHECK_THROW(Program::getInstance().addFunction(func), std::runtime_error)
	BOOST_CHECK_THROW(Program::getInstance().getFunction("not_exists"), std::runtime_error);
}



BOOST_AUTO_TEST_CASE(ProgramTest_nubmerOfFunctions)
{
	Program::getInstance().clear();

	Function func;
	func.setName("f1");

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 0);

	Program::getInstance().addFunction(func);

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 1);

	func.setName("f2");
	Program::getInstance().addFunction(func);

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 2);
}




/**
Проверка метода Program::getFunctionNames()
*/

BOOST_AUTO_TEST_CASE(ProgramTest_getFunctionNames)
{
	Program::getInstance().clear();

	Function func;
	func.setName("f1");

	Program::getInstance().addFunction(func);

	std::list<std::string> lst1, lst2;
	lst1.push_front("f1");

	lst2 = Program::getInstance().getFunctionNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst2.begin(), lst2.end());

	func.setName("f2");
	lst1.push_front("f2");
	Program::getInstance().addFunction(func);

	lst2 = Program::getInstance().getFunctionNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst2.begin(), lst2.end());

	func.setName("f3");
	lst1.push_front("f3");
	Program::getInstance().addFunction(func);

	lst2 = Program::getInstance().getFunctionNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst2.begin(), lst2.end());

}



BOOST_AUTO_TEST_SUITE_END();
