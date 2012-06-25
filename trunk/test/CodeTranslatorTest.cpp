#include <stdexcept>
#include <ctime>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include "../src/CodeTranslator.h"
#include "../src/ParseError.h"


/**
@file CodeTranslatorTest.cpp
@brief Набор модульных тестов для класса CodeTranslator
*/


BOOST_AUTO_TEST_SUITE(CODETRANSLATOR_TEST_SUITE);


/**
Тест функций setDataKeeper, getDataKeeper
*/

BOOST_AUTO_TEST_CASE(VarTranslatorTest_true)
{
	DataKeeper keeper;
	keeper.addVar(Value(), "name");

	CodeTranslator translator;
	translator.setDataKeeper(keeper);

	CodeTranslator translator_ = translator;
	DataKeeper keeper_ = translator_.getDataKeeper();

	BOOST_CHECK_EQUAL(keeper_.isExists("name"), true);
	BOOST_CHECK_EQUAL(keeper_.isVar("name"), true);	
	BOOST_CHECK_EQUAL(keeper_.isArray("name"), false);	
}


BOOST_AUTO_TEST_SUITE_END();
