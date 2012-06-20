#include <stdexcept>
#include <ctime>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include "../src/VarTranslator.h"
#include "../src/ParseError.h"


/**
@file VarTranslatorTest.cpp
@brief Набор модульных тестов для класса VarTranslator
*/


BOOST_AUTO_TEST_SUITE(VARTRANSLATOR_TEST_SUITE);


/**
Тест на корректную трансляцию определений переменных при правильном входе.
*/

BOOST_AUTO_TEST_CASE(VarTranslatorTest_true)
{
	VarTranslator translator;
	translator.translate("uchar ch 125");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);

	BOOST_CHECK_EQUAL(translator.getName(), "ch");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 125);

	translator.translate("const mod8 ch_ 512");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch_");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 0);

	translator.translate("schar ch__");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch__");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), false);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_THROW(translator.getValue().getValue(), std::runtime_error);
}



/**
Тест на корректную работу при неверном входе при трансляции определений переменных.
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


/**
Тест на корректную работу при верном входе при трансляции определений массивов.
*/


BOOST_AUTO_TEST_CASE(VarTranslatorTest_arrTrue)
{
	VarTranslator translator;
	translator.translate("const array const uchar arr_name 5 256 2 3 4 5");

	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getName(), "arr_name");
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 5);
	BOOST_CHECK_EQUAL(translator.getArray()[0].getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(translator.getArray()[4].isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getArray()[0].getValue(), 0);
	BOOST_CHECK_EQUAL(translator.getArray()[1].getValue(), 2);
	BOOST_CHECK_EQUAL(translator.getArray()[2].getValue(), 3);
	BOOST_CHECK_EQUAL(translator.getArray()[3].getValue(), 4);
	BOOST_CHECK_EQUAL(translator.getArray()[4].getValue(), 5);
	BOOST_CHECK_EQUAL(translator.getArray()[4].isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getArray()[4].isWriteable(), false);

	translator.translate("array mod8 arr_ 0");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getName(), "arr_");
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 0);


	translator.translate("array mod8 arr 1");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getName(), "arr");
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 1);
	BOOST_CHECK_EQUAL(translator.getArray()[0].getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isReadable(), false);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isWriteable(), true);

	translator.translate("const array mod8 arr 1 1");
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isWriteable(), true);
}

/**
Тест на корректную работу при неверном входе при трансляции определений массивов.
*/


BOOST_AUTO_TEST_CASE(VarTranslatorTest_arrFalse)
{
	VarTranslator translator;
	BOOST_CHECK_THROW(translator.translate("const array const wrong_type name 5"), ParseError);
	BOOST_CHECK_THROW(translator.translate("array const_ uchar name 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("array uchar name"), ParseError);
	BOOST_CHECK_THROW(translator.translate("Array mod8 name 1"), ParseError);
}





BOOST_AUTO_TEST_CASE(VarTranslatorTest_strTrue)
{
	VarTranslator translator;
	translator.translate("const array mod8 str \"val\"");
	BOOST_CHECK_EQUAL(translator.getName(), "str");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 3);
	BOOST_CHECK_EQUAL(translator.getArray()[0].getValue(), 'v');
	BOOST_CHECK_EQUAL(translator.getArray()[0].isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getArray()[1].getValue(), 'a');
	BOOST_CHECK_EQUAL(translator.getArray()[2].getValue(), 'l');
	BOOST_CHECK_EQUAL(translator.getArray()[2].getType(), Value::MOD8);

	translator.translate("array const uchar name \"vv\"");
	BOOST_CHECK_EQUAL(translator.getName(), "name");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 2);
	BOOST_CHECK_EQUAL(translator.getArray()[0].getValue(), 'v');
	BOOST_CHECK_EQUAL(translator.getArray()[1].getValue(), 'v');
	BOOST_CHECK_EQUAL(translator.getArray()[0].isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getArray()[1].isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getArray()[1].isWriteable(), false);
}




BOOST_AUTO_TEST_CASE(VarTranslatorTest_strFalse)
{
	VarTranslator translator;
	BOOST_CHECK_THROW(translator.translate("array uchar name"), ParseError);
	BOOST_CHECK_THROW(translator.translate("array  name \"\""), ParseError);
	BOOST_CHECK_THROW(translator.translate("array  name const \"\""), ParseError);
	BOOST_CHECK_THROW(translator.translate("const array uint name \"\""), ParseError);

}



BOOST_AUTO_TEST_SUITE_END();

