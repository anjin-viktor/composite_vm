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
Тест конструкторов и функции Command::setOperationType();
*/

BOOST_AUTO_TEST_CASE(CommandTest_GetSetType)
{
	Command com1, com2(Command::ADD);
	com1.setOperationType(Command::ADD);
	BOOST_CHECK_EQUAL(com1.getOperationType(), com2.getOperationType());
}


/**
Тест функции Command::strToOperation()
*/

BOOST_AUTO_TEST_CASE(CommandTest_strToOperation)
{
	BOOST_CHECK_EQUAL(Command::strToOperation("MOV"), Command::MOV);
	BOOST_CHECK_EQUAL(Command::strToOperation("mov"), Command::MOV);
	BOOST_CHECK_EQUAL(Command::strToOperation("MoV"), Command::MOV);

	BOOST_CHECK_EQUAL(Command::strToOperation("ADD"), Command::ADD);
	BOOST_CHECK_EQUAL(Command::strToOperation("SUB"), Command::SUB);
	BOOST_CHECK_EQUAL(Command::strToOperation("MUL"), Command::MUL);
	BOOST_CHECK_EQUAL(Command::strToOperation("DIV"), Command::DIV);
	BOOST_CHECK_EQUAL(Command::strToOperation("MOD"), Command::MOD);
	BOOST_CHECK_EQUAL(Command::strToOperation("CALL"), Command::CALL);
	BOOST_CHECK_EQUAL(Command::strToOperation("RET"), Command::RET);
	BOOST_CHECK_EQUAL(Command::strToOperation("JMP"), Command::JMP);
	BOOST_CHECK_EQUAL(Command::strToOperation("JE"), Command::JE);
	BOOST_CHECK_EQUAL(Command::strToOperation("JL"), Command::JL);
	BOOST_CHECK_EQUAL(Command::strToOperation("JG"), Command::JG);
	BOOST_CHECK_EQUAL(Command::strToOperation("JNE"), Command::JNE);
	BOOST_CHECK_EQUAL(Command::strToOperation("NOP"), Command::NOP);
	BOOST_CHECK_EQUAL(Command::strToOperation("NOPP"), Command::NONE);
	BOOST_CHECK_EQUAL(Command::strToOperation("NONE"), Command::NONE);
	BOOST_CHECK_EQUAL(Command::strToOperation(" mov "), Command::NONE);	
}


BOOST_AUTO_TEST_SUITE_END();
