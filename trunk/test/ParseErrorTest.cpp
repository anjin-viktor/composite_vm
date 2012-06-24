#include <stdexcept>

#include <boost/test/unit_test.hpp>
#include "../src/ParseError.h"


#include <iostream>

/**
@file ParseErrorTest.cpp
@brief Набор модульных тестов для класса ParseError.
*/




BOOST_AUTO_TEST_SUITE(PARSEERROR_TEST_SUITE);


/**
Функция для тестирования конструктора, методов ParseError::what и ParseError::msg.
*/

BOOST_AUTO_TEST_CASE(ParseErrorTest)
{
	ParseError err("error msg");
	BOOST_CHECK_EQUAL(err.what(), "error msg");

	err.setMsg("msg");
	BOOST_CHECK_EQUAL(err.what(), "msg");
}



BOOST_AUTO_TEST_SUITE_END();
