#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/VarOperand.h"



/**
@file VarOperandTest.cpp
@brief Набор модульных тестов для класса VarOperand.
*/


BOOST_AUTO_TEST_SUITE(VAROPERAND_TEST_SUITE);


/**
Тест функций VarOperand::setType.
*/

BOOST_AUTO_TEST_CASE(VarOperandTest_1)
{
	VarOperand op(NULL, Value::NO_TYPE);
	op.setType(Value::UNSIGNED_CHAR);
}

/**
Тест функций set/getValue()
*/
BOOST_AUTO_TEST_CASE(VarOperandTest_2)
{
	Value val(0, Value::UNSIGNED_CHAR, true, true);
	VarOperand op(&val, Value::NO_TYPE);
	op.setType(Value::UNSIGNED_CHAR);

	op.setValue(256);

	BOOST_CHECK_EQUAL(op.getValue(), 0);

}


BOOST_AUTO_TEST_SUITE_END();
