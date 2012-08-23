#include <boost/test/unit_test.hpp>

#include "../src/HeaderTranslator.h"
#include "../src/DataKeeper.h"
#include "../src/ParseError.h"
#include "../src/Value.h"


/**
@file HeaderTranslatorTest.cpp
@brief Набор модульных тестов для класса HeaderTranslator
*/


BOOST_AUTO_TEST_SUITE(HeaderTranslator_TEST_SUITE);



/**
Тест конструктора
*/
BOOST_AUTO_TEST_CASE(HeaderTranslatorSetGet_Test)
{
	HeaderTranslator tr;

	BOOST_CHECK_EQUAL(tr.getName(), "");
}



/**
Тест на верную работу при корректном входе
*/
BOOST_AUTO_TEST_CASE(HeaderTranslator_false)
{
	HeaderTranslator translator;
	DataKeeper keeper;

	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_NO_THROW(translator.translate(".name main"));
	BOOST_CHECK_NO_THROW(translator.translate("   .name    _main"));
	BOOST_CHECK_NO_THROW(translator.translate("   .name    _MAIN"		   ));
	BOOST_CHECK_NO_THROW(translator.translate(".name name uchar ch"));
	BOOST_CHECK_NO_THROW(translator.translate(".name name uchar ch, sint n"));
	BOOST_CHECK_NO_THROW(translator.translate(".name name uchar ch,sint n"));
	BOOST_CHECK_NO_THROW(translator.translate(".name 	 name     uchar  ch	 ,   	sint	 v 	"));
	BOOST_CHECK_NO_THROW(translator.translate(".name name array sshort arr"));
	BOOST_CHECK_NO_THROW(translator.translate(".name name array sint arr, ARRAY uint arr1"));
	BOOST_CHECK_NO_THROW(translator.translate(".name 	 name    const mod16  ch	 ,   	sint	 v , ARRAY schar arr "));
}




/**
Тест на порождение исключений при неверном входе
*/
BOOST_AUTO_TEST_CASE(HeaderTranslator_true)
{
	HeaderTranslator translator;
	DataKeeper keeper;

	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_THROW(translator.translate(".name"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".nam  _main"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name    1MAIN"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main:"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main error_type ch"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main uint ch uchar ch1"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main array arr"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main const array arr"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main const uchar"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".name main const uchar ch, ARRay arr"), ParseError);
}




/**
Тест правильности получения имени модуля
*/
BOOST_AUTO_TEST_CASE(HeaderTranslator_moduleName)
{
	HeaderTranslator translator;
	DataKeeper keeper;

	translator.setDataKeeperPtr(&keeper);

	translator.translate(".name main");
	BOOST_CHECK_EQUAL(translator.getName(), "main");

	translator.translate("   .name    _main");
	BOOST_CHECK_EQUAL(translator.getName(), "_main");

	translator.translate("   .name    _MAIN  		");
	BOOST_CHECK_EQUAL(translator.getName(), "_MAIN");

	translator.translate(".name name uchar ch");
	BOOST_CHECK_EQUAL(translator.getName(), "name");

	translator.translate(".name na_me uchar ch, sint n");
	BOOST_CHECK_EQUAL(translator.getName(), "na_me");

	translator.translate(".name name uchar ch,sint n");
	BOOST_CHECK_EQUAL(translator.getName(), "name");

	translator.translate(".name 	 name     uchar  ch	 ,   	sint	 v 	");
	BOOST_CHECK_EQUAL(translator.getName(), "name");

	translator.translate(".name name array sshort arr");
	BOOST_CHECK_EQUAL(translator.getName(), "name");

	translator.translate(".name name array sint arr, ARRAY uint arr1");
	BOOST_CHECK_EQUAL(translator.getName(), "name");

	translator.translate(".name 	 name    const mod16  ch	 ,   	sint	 v , ARRAY schar arr ");	
	BOOST_CHECK_EQUAL(translator.getName(), "name");
}



/**
Проверка трансляции переменных
*/
BOOST_AUTO_TEST_CASE(HeaderTranslator_var)
{
	HeaderTranslator translator;
	DataKeeper keeper;

	translator.setDataKeeperPtr(&keeper);

	translator.translate(".name main mod8 ch");
	BOOST_CHECK_EQUAL(keeper.isVar("ch"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("ch"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch").getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch").isWriteable(), true);
	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 1);

	translator.translate(".name 	 name     uchar  ch1	 ,   	sint	 v, mod8 s");
	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 3);
	BOOST_CHECK_EQUAL(keeper.isVar("ch1"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("ch1"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch1").getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch1").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch1").isWriteable(), true);

	BOOST_CHECK_EQUAL(keeper.isVar("v"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("v"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("v").getType(), Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(keeper.getVarValue("v").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("v").isWriteable(), true);

	BOOST_CHECK_EQUAL(keeper.isVar("s"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("s"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("s").getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(keeper.getVarValue("s").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("s").isWriteable(), true);

	translator.translate(".name name sshort  sh1, const sint v");
	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 2);
	BOOST_CHECK_EQUAL(keeper.isVar("sh1"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("sh1"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("sh1").getType(), Value::SIGNED_SHORT);
	BOOST_CHECK_EQUAL(keeper.getVarValue("sh1").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("sh1").isWriteable(), true);

	BOOST_CHECK_EQUAL(keeper.isVar("v"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("v"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("v").getType(), Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(keeper.getVarValue("v").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("v").isWriteable(), false);
}



/**
Проветка трансляции массивов
*/
BOOST_AUTO_TEST_CASE(HeaderTranslator_arr)
{
	HeaderTranslator translator;
	DataKeeper keeper;

	translator.setDataKeeperPtr(&keeper);

	translator.translate(".name main array mod8 str");

	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 1);
	BOOST_CHECK_EQUAL(keeper.isArray("str"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("str"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("str").getType(), Value::MOD8);


	translator.translate(".name main array mod8 str, ARRAY sint numbers  , array     ushort   test ");
	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 3);
	BOOST_CHECK_EQUAL(keeper.isArray("str"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("str"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("str").getType(), Value::MOD8);

	BOOST_CHECK_EQUAL(keeper.isArray("numbers"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("numbers"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("numbers").getType(), Value::SIGNED_INT);

	BOOST_CHECK_EQUAL(keeper.isArray("test"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("test"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("test").getType(), Value::UNSIGNED_SHORT);
}



/**
Проверка трансляции переменных и массивов вместе
*/
BOOST_AUTO_TEST_CASE(HeaderTranslator_arrVar)
{
	HeaderTranslator translator;
	DataKeeper keeper;

	translator.setDataKeeperPtr(&keeper);

	translator.translate(".name main array mod8 str, const schar ch, sint n");

	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 3);
	BOOST_CHECK_EQUAL(keeper.isArray("str"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("str"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("str").getType(), Value::MOD8);

	BOOST_CHECK_EQUAL(keeper.isVar("ch"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("ch"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch").getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("ch").isWriteable(), false);

	BOOST_CHECK_EQUAL(keeper.isVar("n"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("n"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("n").getType(), Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(keeper.getVarValue("n").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("n").isWriteable(), true);



	translator.translate(".name main mod8 a, array mod8 str, ARRAY sint numbers  , const mod32 b, array     ushort   test ");
	BOOST_CHECK_EQUAL(keeper.getNumberOfElements(), 5);
	BOOST_CHECK_EQUAL(keeper.isArray("str"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("str"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("str").getType(), Value::MOD8);

	BOOST_CHECK_EQUAL(keeper.isArray("numbers"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("numbers"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("numbers").getType(), Value::SIGNED_INT);

	BOOST_CHECK_EQUAL(keeper.isArray("test"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("test"), false);
	BOOST_CHECK_EQUAL(keeper.getArray("test").getType(), Value::UNSIGNED_SHORT);



	BOOST_CHECK_EQUAL(keeper.isVar("a"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("a"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("a").getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(keeper.getVarValue("a").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("a").isWriteable(), true);

	BOOST_CHECK_EQUAL(keeper.isVar("b"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("b"), false);
	BOOST_CHECK_EQUAL(keeper.getVarValue("b").getType(), Value::MOD32);
	BOOST_CHECK_EQUAL(keeper.getVarValue("b").isReadable(), true);
	BOOST_CHECK_EQUAL(keeper.getVarValue("b").isWriteable(), false);
}




/**
Тест функции getNameFromString
*/


BOOST_AUTO_TEST_CASE(HeaderTranslator_getNameFromStr_test)
{
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr(".name main"), "main");
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr("    .name    main   "), "main");
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr(".name _main"), "_main");
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr(".name _m1"), "_m1");
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr(".name m1"), "m1");
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr(".name main 123 asd"), "main");
	BOOST_CHECK_EQUAL(HeaderTranslator::getNameFromStr(".name main mod8 a, array mod8 str, ARRAY sint numbers  , const mod32 b, array     ushort   test "), "main");



	BOOST_CHECK_THROW(HeaderTranslator::getNameFromStr("main"), std::runtime_error);
	BOOST_CHECK_THROW(HeaderTranslator::getNameFromStr(".name   "), std::runtime_error);
	BOOST_CHECK_THROW(HeaderTranslator::getNameFromStr(".name 1main"), std::runtime_error);
	BOOST_CHECK_THROW(HeaderTranslator::getNameFromStr(".name mai:n"), std::runtime_error);
	BOOST_CHECK_THROW(HeaderTranslator::getNameFromStr("1name main"), std::runtime_error);
	BOOST_CHECK_THROW(HeaderTranslator::getNameFromStr(".name main:"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END();
