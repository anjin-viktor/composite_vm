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

BOOST_AUTO_TEST_CASE(VarOperandTest)
{
	VarOperand op;
	op.setType(Value::UNSIGNED_CHAR);
}



BOOST_AUTO_TEST_SUITE_END();
