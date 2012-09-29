#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/ArrayOperand.h"

#define BOOST_TEST_MODULE tests

/**
@file ArrayOperandTest.cpp
@brief Набор модульных тестов для класса ArrayOperand.
*/


BOOST_AUTO_TEST_SUITE(ARRAYOPERAND_TEST_SUITE);


/**
Тест конструкторов
*/

BOOST_AUTO_TEST_CASE(ArrayOperandTest_1)
{
	Array arr;
	ArrayOperand op, op1(&arr);
	
	BOOST_CHECK_EQUAL(op.getArrayPtr(), static_cast<Array *>(NULL));

	op.setArrayPtr(&arr);
	BOOST_CHECK_EQUAL(op.getArrayPtr(), &arr);
	BOOST_CHECK_EQUAL(op1.getArrayPtr(), &arr);
}



/**
Тест функции hasValue
*/

BOOST_AUTO_TEST_CASE(ArrayOperandTest_2)
{
	Array arr;
	ArrayOperand op, op1(&arr);
	
	BOOST_CHECK_EQUAL(op.hasValue(), false);

	op.setArrayPtr(&arr);
	BOOST_CHECK_EQUAL(op.hasValue(), true);
	BOOST_CHECK_EQUAL(op1.hasValue(), true);	


	op1.setArrayPtr(NULL);
	BOOST_CHECK_EQUAL(op1.hasValue(), false);
}


BOOST_AUTO_TEST_SUITE_END();
