#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/Command.h"



/**
@file CommandTest.cpp
@brief Набор модульных тестов для класса Command
*/


BOOST_AUTO_TEST_SUITE(COMMAND_TEST_SUITE);


/**
Тест функции VarOperand::setType.
*/

BOOST_AUTO_TEST_CASE(CommandTest_GetSetType)
{
	Command com1, com2(Command::ADD);
	com1.setOperationType(Command::ADD);
	BOOST_CHECK_EQUAL(com1.getOperationType(), com2.getOperationType());
}



BOOST_AUTO_TEST_SUITE_END();
