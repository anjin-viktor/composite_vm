#include <boost/test/unit_test.hpp>

#include "../src/CallOperand.h"
#include "../src/Value.h"
#include "../src/Array.h"


/**
@file CallOperandTest.cpp
@brief Набор модульных тестов для класса CallOperand
*/


BOOST_AUTO_TEST_SUITE(CALLOPERAND_TEST_SUITE);

/**
Проверка работы с переменными
*/
BOOST_AUTO_TEST_CASE(CallOperand_test_1)
{
	CallOperand op;

	Value val(1);

	op.setValuePtr(&val);

	BOOST_CHECK_EQUAL(op.isValue(), true);
	BOOST_CHECK_EQUAL(op.isArray(), false);

	BOOST_CHECK_EQUAL(op.getValue().getValue(), 1);
	op.getValue().setValue(2);
	BOOST_CHECK_EQUAL(op.getValue().getValue(), 2);
	BOOST_CHECK_EQUAL(val.getValue(), 2);

	Value v = val.createNoLink();
	op.setValuePtr(&(v));
	BOOST_CHECK_EQUAL(op.isValue(), true);
	BOOST_CHECK_EQUAL(op.getValue().getValue(), 2);
	op.getValue().setValue(1);
	BOOST_CHECK_EQUAL(op.getValue().getValue(), 1);
	BOOST_CHECK_EQUAL(val.getValue(), 2);
}


/**
Проверка работы с массивами
*/
BOOST_AUTO_TEST_CASE(CallOperand_test_2)
{
	CallOperand op;
	
	Array arr(3, Value::MOD8);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;

	op.setArrayPtr(&arr);

	BOOST_CHECK_EQUAL(op.isValue(), false);
	BOOST_CHECK_EQUAL(op.isArray(), true);

	BOOST_CHECK_EQUAL(op.getArray()[0].getValue(), 1);
	BOOST_CHECK_EQUAL(op.getArray()[1].getValue(), 2);
	BOOST_CHECK_EQUAL(op.getArray()[2].getValue(), 3);
	BOOST_CHECK_EQUAL(op.getArray().size(), 3);
}


/**
Тест метода convert
*/
BOOST_AUTO_TEST_CASE(CallOperand_test_3)
{
	DataKeeper keeper, newKeeper;
	keeper.addVar(Value(1, Value::MOD16, true, true), "var1");
	keeper.addVar(Value(2, Value::MOD16, true, true), "var2");

	newKeeper.addVar(Value(1, Value::MOD16, true, true), "var1");
	newKeeper.addVar(Value(2, Value::MOD16, true, true), "var2");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(2, Value::MOD16, true, true);
	array[2] = Value(3, Value::MOD16, true, true);

	keeper.addArray(array, "arr1");
	newKeeper.addArray(array, "arr1");

	CallOperand cop;

	cop.setValuePtr(&(keeper.getVarValue("var1")));
	cop.getValue().setValue(3);
	BOOST_CHECK_EQUAL(keeper.getVarValue("var1").getValue(), 3);

	cop = cop.convert(&keeper, &newKeeper);

	cop.getValue().setValue(4);
	BOOST_CHECK_EQUAL(keeper.getVarValue("var1").getValue(), 3);
	BOOST_CHECK_EQUAL(newKeeper.getVarValue("var1").getValue(), 4);
}




BOOST_AUTO_TEST_SUITE_END();
