#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/VarTranslator.h"


#include <iostream>

/**
@file VarTranslatorTest.cpp
@brief Набор модульных тестов для класса VarTranslator
*/


BOOST_AUTO_TEST_SUITE(VARTRANSLATOR_TEST_SUITE);


/**
тест функции VarTranslator::translate
*/

BOOST_AUTO_TEST_CASE(VarTranslatorTest)
{
	VarTranslator translator;
	std::pair<std::string, Value> res = translator.translate("uchar ch 125");
	BOOST_CHECK_EQUAL(res.first, "ch");
	BOOST_CHECK_EQUAL(res.second.isReadable(), true);
	BOOST_CHECK_EQUAL(res.second.isWriteable(), true);
	BOOST_CHECK_EQUAL(res.second.getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(res.second.getValue(), 125);
}



BOOST_AUTO_TEST_SUITE_END();
