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

	translator.translate("const sint ch___ 1000000");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch___");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 1000000);

	translator.translate("uint ch____ 1000000");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch____");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::UNSIGNED_INT);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 1000000);

	translator.translate("mod32 ch_____ 1000000");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch_____");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::MOD32);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 1000000);


	translator.translate("const sshort ch______ -10000");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch______");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), false);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::SIGNED_SHORT);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), -10000);





	translator.translate("ushort ch_______ 65535");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch_______");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::UNSIGNED_SHORT);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 65535);





	translator.translate("mod16 ch________ 65537");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);
	BOOST_CHECK_EQUAL(translator.getName(), "ch________");
	BOOST_CHECK_EQUAL(translator.getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getValue().getType(), Value::MOD16);
	BOOST_CHECK_EQUAL(translator.getValue().getValue(), 1);
}



/**
Тест на корректную работу при неверном входе при трансляции определений переменных.
*/


BOOST_AUTO_TEST_CASE(VarTranslatorTest_false)
{
	VarTranslator translator;
	BOOST_CHECK_THROW(translator.translate("int val1"), ParseError);
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
	translator.clear();

	translator.translate("array mod8 arr_ 0");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getName(), "arr_");
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 0);
	translator.clear();


	translator.translate("array mod8 arr 1");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), true);
	BOOST_CHECK_EQUAL(translator.getName(), "arr");
	BOOST_CHECK_EQUAL(translator.getArray().isWriteable(), true);
	BOOST_CHECK_EQUAL(translator.getArray().size(), 1);
	BOOST_CHECK_EQUAL(translator.getArray()[0].getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isReadable(), false);
	BOOST_CHECK_EQUAL(translator.getArray()[0].isWriteable(), true);
	translator.clear();

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



/**
Тест на корректную работу при верхом входе при трансляции определений строк.
*/


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
	translator.clear();


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


/**
Тест на корректную работу при неверхом входе при трансляции определений строк.
*/


BOOST_AUTO_TEST_CASE(VarTranslatorTest_strFalse)
{
	VarTranslator translator;
	BOOST_CHECK_THROW(translator.translate("array uchar name"), ParseError);
	BOOST_CHECK_THROW(translator.translate("array  name \"\""), ParseError);
	BOOST_CHECK_THROW(translator.translate("array  name const \"\""), ParseError);
	BOOST_CHECK_THROW(translator.translate("const array uint name \"\""), ParseError);

}


/**
Тест поведения пре переопределении переменных.
*/

BOOST_AUTO_TEST_CASE(VarTranslatorTest_redifinition)
{
	VarTranslator translator;
	translator.translate("uchar char 1");
	BOOST_CHECK_THROW(translator.translate("mod8 char 2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("array mod8 char 2 0 1"), ParseError);
	translator.clear();


	translator.translate("array mod8 char 2 0 1");
	BOOST_CHECK_THROW(translator.translate("uchar char 2"), ParseError);
}


/**
Проверка трансляции на наборе из нескольких строк.
*/

BOOST_AUTO_TEST_CASE(VarTranslatorTest_combined)
{
	VarTranslator translator;
	std::string str1 = "const uchar ch";
	std::string str2 = "const uchar ch1 1";
	std::string str3 = "uint val 65536";
	std::string str4 = "const array mod8 arr 10 0 1 2 3 4 5 6 7 8 9";
	std::string str5 = "array const schar name \"name\"";
	std::string str6 = "sint val1";
	std::string str7 = "array mod16 tmp 1";
	std::string str8 = "const array const sint arr_name 5";


	translator.translate(str1);
	translator.translate(str2);
	translator.translate(str3);
	translator.translate(str4);
	translator.translate(str5);
	translator.translate(str6);
	translator.translate(str7);
	translator.translate(str8);


	DataKeeper keeper = translator.getDataKeeper();

	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 8);

	BOOST_CHECK_EQUAL(keeper.isExists("ch"), true);
	BOOST_CHECK_EQUAL(keeper.isExists("ch1"), true);
	BOOST_CHECK_EQUAL(keeper.isExists("val"), true);
	BOOST_CHECK_EQUAL(keeper.isExists("val1"), true);
	BOOST_CHECK_EQUAL(keeper.isExists("arr"), true);
	BOOST_CHECK_EQUAL(keeper.isExists("name"), true);


	Value val = keeper.getVarValue("ch");

	BOOST_CHECK_EQUAL(val.isReadable(), false);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_CHAR);

	val = keeper.getVarValue("ch1");
	BOOST_CHECK_EQUAL(val.isReadable(), true);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(val.getValue(), 1);

	val = keeper.getVarValue("val");
	BOOST_CHECK_EQUAL(val.isReadable(), true);
	BOOST_CHECK_EQUAL(val.isWriteable(), true);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_INT);
	BOOST_CHECK_EQUAL(val.getValue(), 65536);


	val = keeper.getVarValue("val1");
	BOOST_CHECK_EQUAL(val.isReadable(), false);
	BOOST_CHECK_EQUAL(val.isWriteable(), true);
	BOOST_CHECK_EQUAL(val.getType(), Value::SIGNED_INT);


	Array arr = keeper.getArray("arr");
	BOOST_CHECK_EQUAL(arr.isWriteable(), false);
	BOOST_CHECK_EQUAL(arr.size(), 10);

	for(int i=0; i<10; i++)
	{
		BOOST_CHECK_EQUAL(arr[i].isReadable(), true);
		BOOST_CHECK_EQUAL(arr[i].isWriteable(), true);
		BOOST_CHECK_EQUAL(arr[i].getType(), Value::MOD8);
		BOOST_CHECK_EQUAL(arr[i].getValue(), i);
	}


	arr = keeper.getArray("name");
	BOOST_CHECK_EQUAL(arr.isWriteable(), true);
	BOOST_CHECK_EQUAL(arr.size(), 4);

	BOOST_CHECK_EQUAL(arr[0].isReadable(), true);
	BOOST_CHECK_EQUAL(arr[0].isWriteable(), false);
	BOOST_CHECK_EQUAL(arr[0].getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(arr[0].getValue(), 'n');

	BOOST_CHECK_EQUAL(arr[1].isReadable(), true);
	BOOST_CHECK_EQUAL(arr[1].isWriteable(), false);
	BOOST_CHECK_EQUAL(arr[1].getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(arr[1].getValue(), 'a');

	BOOST_CHECK_EQUAL(arr[2].isReadable(), true);
	BOOST_CHECK_EQUAL(arr[2].isWriteable(), false);
	BOOST_CHECK_EQUAL(arr[2].getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(arr[2].getValue(), 'm');

	BOOST_CHECK_EQUAL(arr[3].isReadable(), true);
	BOOST_CHECK_EQUAL(arr[3].isWriteable(), false);
	BOOST_CHECK_EQUAL(arr[3].getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(arr[3].getValue(), 'e');



	arr = keeper.getArray("tmp");
	BOOST_CHECK_EQUAL(arr.isWriteable(), true);
	BOOST_CHECK_EQUAL(arr.size(), 1);

	BOOST_CHECK_EQUAL(arr[0].isReadable(), false);
	BOOST_CHECK_EQUAL(arr[0].isWriteable(), true);
	BOOST_CHECK_EQUAL(arr[0].getType(), Value::MOD16);



	arr = keeper.getArray("arr_name");
	BOOST_CHECK_EQUAL(arr.isWriteable(), false);
	BOOST_CHECK_EQUAL(arr.size(), 5);

	for(int i=0; i<5; i++)
	{
		BOOST_CHECK_EQUAL(arr[i].isReadable(), false);
		BOOST_CHECK_EQUAL(arr[i].isWriteable(), false);
		BOOST_CHECK_EQUAL(arr[i].getType(), Value::SIGNED_INT);
	}

	BOOST_CHECK_THROW(arr[5], std::runtime_error);
}



BOOST_AUTO_TEST_CASE(VarTranslatorTest_comment)
{
	VarTranslator translator;
	translator.translate("uchar ch;comment");
	BOOST_CHECK_EQUAL(translator.isVariable(), true);
	BOOST_CHECK_EQUAL(translator.isArray(), false);

	translator.translate(";comment");
	BOOST_CHECK_EQUAL(translator.isVariable(), false);
	BOOST_CHECK_EQUAL(translator.isArray(), false);

}



BOOST_AUTO_TEST_SUITE_END();
