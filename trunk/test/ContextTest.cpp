#include <vector>
#include <stdexcept>
#include <iostream>
#include <stack>

#include <boost/test/unit_test.hpp>

#include "../src/Context.h"
#include "../src/Command.h"
#include "../src/Translator.h"
#include "../src/DataKeeper.h"
#include "../src/Program.h"

/**
@file ArrayTest.cpp
@brief Набор модульных тестов для класса Context
*/



BOOST_AUTO_TEST_SUITE(Context_TEST_SUITE);



BOOST_AUTO_TEST_CASE(Context_test)
{
	Function f;
	std::vector<Command> code;

	f.setExceptionHandler(Exception::NumericError, code);

	Command c;
	c.setOperationType(Command::NOP);
	code.push_back(c);

	f.setCommands(code);

	Context cntx;
	cntx.setFunction(f.copy());
	cntx.init();

	BOOST_CHECK_EQUAL(cntx.atEnd(), false);
	BOOST_CHECK_EQUAL(cntx.handlerIsExists(Exception::NumericError), true);
	BOOST_CHECK_EQUAL(cntx.handlerIsExists(Exception::ConstraintError), false);

	cntx.execHandler(Exception::NumericError);

	BOOST_CHECK_EQUAL(cntx.atEnd(), true);
	BOOST_CHECK_THROW(cntx.execHandler(Exception::ConstraintError), std::runtime_error);
}



/**
Тест копирования в контекст функции от загрузчика
*/



BOOST_AUTO_TEST_CASE(Context_test_1)
{
	Translator tr;

	tr.setInputFileName("ContextTestFiles/1.mpr");

	BOOST_CHECK_NO_THROW(tr.translate());

	DataKeeper *pkeeper = Program::getInstance().getFunction("main").getDataKeeperPtr();

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);

	Context cntx;
	cntx.setFunction(Program::getInstance().getFunction("main").copy());
	pkeeper = cntx.getFunctionPtr() -> getDataKeeperPtr();


	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);



	cntx.init();
	pkeeper = cntx.getFunctionPtr() -> getDataKeeperPtr();


	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);

	std::stack<Context>	 stack;
	stack.push(cntx);

	pkeeper = stack.top().getFunctionPtr() -> getDataKeeperPtr();
	std::vector<Command> code = stack.top().m_function.getCommands();
	
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").getValue(), 70);

	BOOST_CHECK_EQUAL(stack.top().m_code[stack.top().m_ip].getOperationType(), Command::MOV);

	boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
		(stack.top().m_code[stack.top().m_ip].getFirstOperand());
	boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
		(stack.top().m_code[stack.top().m_ip].getSecondOperand());


	BOOST_CHECK_EQUAL(pfArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isWriteable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isConstant(), false);
	BOOST_CHECK_EQUAL(pfArg -> isCast(), false);
	BOOST_CHECK_EQUAL(pfArg -> getValue(), 70);

	BOOST_CHECK_EQUAL(psArg -> isWriteable(), false);
	BOOST_CHECK_EQUAL(psArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(psArg -> getValue(), 72);
}




BOOST_AUTO_TEST_CASE(Context_test_1_)
{
	Translator tr;
	DataKeeper *pkeeper;

	tr.setInputFileName("ContextTestFiles/1.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	Function func;
	
	{
		Function f = Program::getInstance().getFunction("main").copy();
		func = f;
	}
	Context cntx;
	cntx.setFunction(Program::getInstance().getFunction("main").copy());
	cntx.init();
	pkeeper = cntx.getFunctionPtr() -> getDataKeeperPtr();


	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);


	Program::getInstance().getFunction("main").copy();

	std::vector<Command> code;
	code = Program::getInstance().getFunction("main").getCommands();
	code = cntx.m_function.getCommands();

	boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
		(code[0].getFirstOperand());
	boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
		(code[0].getSecondOperand());

	BOOST_CHECK_EQUAL(code[0].getNumberOfOperands(), 2);
	BOOST_CHECK_EQUAL(code[0].getOperationType(), Command::MOV);
	BOOST_CHECK_EQUAL(pfArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isWriteable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isConstant(), false);
	BOOST_CHECK_EQUAL(pfArg -> isCast(), false);
	BOOST_CHECK_EQUAL(pfArg -> getValue(), 70);

	BOOST_CHECK_EQUAL(psArg -> isWriteable(), false);
	BOOST_CHECK_EQUAL(psArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(psArg -> getValue(), 72);
}

BOOST_AUTO_TEST_SUITE_END();
