#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "../src/CodeExecuter.h"


/**
@file CodeExecuterTest.cpp
@brief Набор модульных тестов для класса CodeExecuter
*/


BOOST_AUTO_TEST_SUITE(CodeExecuter_TEST_SUITE);


/**
Тест #1
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_1)
{
	CodeExecuter exec;
	BOOST_CHECK_THROW(exec.exec(), std::runtime_error)
}



BOOST_AUTO_TEST_SUITE_END();
