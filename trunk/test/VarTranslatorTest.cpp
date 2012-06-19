#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/VarTranslator.h"
#include "../src/ParseError.h"


/**
@file VarTranslatorTest.cpp
@brief Набор модульных тестов для класса VarTranslator
*/


BOOST_AUTO_TEST_SUITE(VARTRANSLATOR_TEST_SUITE);


/**
Тест на корректную работу при правильном входе.
*/

BOOST_AUTO_TEST_CASE(VarTranslatorTest_true)
{
	VarTranslator translator;
	std::pair<std::string, Value> res = translator.translate("uchar ch 125");
	BOOST_CHECK_EQUAL(res.first, "ch");
	BOOST_CHECK_EQUAL(res.second.isReadable(), true);
	BOOST_CHECK_EQUAL(res.second.isWriteable(), true);
	BOOST_CHECK_EQUAL(res.second.getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(res.second.getValue(), 125);


	res = translator.translate("const mod8 ch_ 512");
	BOOST_CHECK_EQUAL(res.first, "ch_");
	BOOST_CHECK_EQUAL(res.second.isReadable(), true);
	BOOST_CHECK_EQUAL(res.second.isWriteable(), false);
	BOOST_CHECK_EQUAL(res.second.getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(res.second.getValue(), 0);


	res = translator.translate("schar ch__");
	BOOST_CHECK_EQUAL(res.first, "ch__");
	BOOST_CHECK_EQUAL(res.second.isReadable(), false);
	BOOST_CHECK_EQUAL(res.second.isWriteable(), true);
	BOOST_CHECK_EQUAL(res.second.getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_THROW(res.second.getValue(), std::runtime_error);
}



/**
Тест на корректную работу при неверном входе.
*/


BOOST_AUTO_TEST_CASE(VarTranslatorTest_false)
{
	VarTranslator translator;
	BOOST_CHECK_THROW(translator.translate("wrong_type ch 125"), ParseError);
	BOOST_CHECK_THROW(translator.translate("uchar 1_wrong_name 125"), ParseError);
	BOOST_CHECK_THROW(translator.translate("uchar ch wrong"), ParseError);
	BOOST_CHECK_THROW(translator.translate("wrong_type ch 125 wrong"), ParseError);
	BOOST_CHECK_THROW(translator.translate("cons uchar ch 125"), ParseError);
	BOOST_CHECK_THROW(translator.translate("const wrong ch 125"), ParseError);
	BOOST_CHECK_THROW(translator.translate("const uchar"), ParseError);
	BOOST_CHECK_THROW(translator.translate("const"), ParseError);
	BOOST_CHECK_THROW(translator.translate("uchar"), ParseError);
	BOOST_CHECK_THROW(translator.translate("ch"), ParseError);
	BOOST_CHECK_THROW(translator.translate("125"), ParseError);

}

BOOST_AUTO_TEST_SUITE_END();
