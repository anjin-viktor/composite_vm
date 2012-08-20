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


/**
Тест функций set/getOperationType()
*/

BOOST_AUTO_TEST_CASE(CommandTest_setGetOperationType)
{
	Command c1(Command::JL), c2;
	BOOST_CHECK_EQUAL(c2.getOperationType(), Command::NONE);
	c2.setOperationType(Command::JL);
	BOOST_CHECK_EQUAL(c1.getOperationType(), c2.getOperationType());
	BOOST_CHECK_EQUAL(c1.getOperationType(), Command::JL);
}



/**
Тест функций работы с операндами
*/

BOOST_AUTO_TEST_CASE(CommandTest_operands)
{
	Command c;

	BOOST_CHECK_EQUAL(c.getFirstOperand().get(), static_cast<Operand *>(NULL));
	BOOST_CHECK_EQUAL(c.getSecondOperand().get(), static_cast<Operand *>(NULL));


	Operand *pop1, *pop2;
	pop1 = new Operand;
	pop2 = new Operand;

	c.setFirstOperand(boost::shared_ptr<Operand>(pop1));

	BOOST_CHECK_EQUAL(c.getFirstOperand().get(), pop1);
	BOOST_CHECK_EQUAL(c.getSecondOperand().get(), static_cast<Operand *>(NULL));

	c.setSecondOperand(boost::shared_ptr<Operand>(pop2));

	BOOST_CHECK_EQUAL(c.getFirstOperand().get(), pop1);
	BOOST_CHECK_EQUAL(c.getSecondOperand().get(), pop2);

	c.setFirstOperand(boost::shared_ptr<Operand>(static_cast<Operand *>(NULL)));
	BOOST_CHECK_EQUAL(c.getFirstOperand().get(), static_cast<Operand *>(NULL));
	BOOST_CHECK_EQUAL(c.getSecondOperand().get(), pop2);
}



BOOST_AUTO_TEST_SUITE_END();
