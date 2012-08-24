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

	op.setValue(val);

	BOOST_CHECK_EQUAL(op.isValue(), true);
	BOOST_CHECK_EQUAL(op.isArray(), false);

	BOOST_CHECK_EQUAL(op.getValue().getValue(), 1);
	op.getValue().setValue(2);
	BOOST_CHECK_EQUAL(op.getValue().getValue(), 2);
	BOOST_CHECK_EQUAL(val.getValue(), 2);


	op.setValue(val.createNoLink());
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

	op.setArray(arr);

	BOOST_CHECK_EQUAL(op.isValue(), false);
	BOOST_CHECK_EQUAL(op.isArray(), true);

	BOOST_CHECK_EQUAL(op.getArray()[0].getValue(), 1);
	BOOST_CHECK_EQUAL(op.getArray()[1].getValue(), 2);
	BOOST_CHECK_EQUAL(op.getArray()[2].getValue(), 3);
	BOOST_CHECK_EQUAL(op.getArray().size(), 3);
}


BOOST_AUTO_TEST_SUITE_END();
