#include <boost/test/unit_test.hpp>
#include "../src/Translator.h"


/**
@file TranslatorTest.cpp
@brief Набор модульных тестов для класса Translator
*/


BOOST_AUTO_TEST_SUITE(Translator_TEST_SUITE);



/**
Тест конструктора и функций set/getInputFileName
*/
BOOST_AUTO_TEST_CASE(TranslatorSetGet_Test)
{
	Translator tr1, tr2("test_name");
	tr1.setInputFileName("test_name_");

	BOOST_CHECK_EQUAL(tr1.getInputFileName(), "test_name_");
	BOOST_CHECK_EQUAL(tr2.getInputFileName(), "test_name");
}





BOOST_AUTO_TEST_SUITE_END();