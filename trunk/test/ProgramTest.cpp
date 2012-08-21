#include <boost/test/unit_test.hpp>
#include "../src/Program.h"


/**
@file ProgramTest.cpp
@brief Набор модульных тестов для класса Program
*/


BOOST_AUTO_TEST_SUITE(Program_TEST_SUITE);



BOOST_AUTO_TEST_CASE(ProgramTest_1)
{
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("test_function"), false);

	Function func;
	func.setName("test_function");
	Program::getInstance().addFunction(func);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("test_function"), true);	
}


BOOST_AUTO_TEST_SUITE_END();
