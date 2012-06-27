#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/ArrayOperand.h"



/**
@file ArrayOperandTest.cpp
@brief Набор модульных тестов для класса ArrayOperand.
*/


BOOST_AUTO_TEST_SUITE(ARRAYOPERAND_TEST_SUITE);


/**
Тест конструктора и функций setArrayPtr(), getArrayPtr()
*/

BOOST_AUTO_TEST_CASE(VarOperandTest)
{
	Array arr;
	ArrayOperand op, op1(&arr);
	
	BOOST_CHECK_EQUAL(op.getArrayPtr(), static_cast<Array *>(NULL));

	op.setArrayPtr(&arr);
	BOOST_CHECK_EQUAL(op.getArrayPtr(), &arr);
	BOOST_CHECK_EQUAL(op1.getArrayPtr(), &arr);

}



BOOST_AUTO_TEST_SUITE_END();
