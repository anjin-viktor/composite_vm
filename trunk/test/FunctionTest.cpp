#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../src/Function.h"
#include "../src/Command.h"
#include "Value.h"
#include "Array.h"
#include "LabelOperand.h"
#include "ArrayOperand.h"
#include "VarOperand.h"
#include "CallOperand.h"


/**
@file FunctionTest.cpp
@brief Набор модульных тестов для класса Function.
*/



BOOST_AUTO_TEST_SUITE(FUNCTION_TEST_SUITE);


/**
Проверка конструктора
*/


BOOST_AUTO_TEST_CASE(testFunction)
{
	Function func;
	func.setName("name");
	BOOST_CHECK_EQUAL(func.getName(), "name");

	std::vector<Command> commands;
	func.setExceptionHandler(Exception::NumericError, commands);
	func.setCommands(commands);
}


/**
Проверка методов set/getCommands
*/

BOOST_AUTO_TEST_CASE(testFunction_setGetCommands)
{
	Function func;
	std::vector<Command> code;

	Command c(Command::NOP);
	c.setLineNumber(1);

	code.push_back(c);
	c.setLineNumber(123);
	code.push_back(c);


	func.setCommands(code);
	std::vector<Command> vect = func.getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(vect.begin(), vect.end(), code.begin(), code.end());
}


/**
Проверка корректности работы со списком аргументов
*/

BOOST_AUTO_TEST_CASE(testFunction_argsList)
{
	Function func;

	std::list<std::string> lst, funcArgs;


	funcArgs = func.getArgsNames();

	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());

	lst.push_front("name1");
	func.addArgName("name1");

	funcArgs = func.getArgsNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());

	lst.push_back("name2");
	func.addArgName("name2");
	lst.push_back("name3");
	func.addArgName("name3");

	funcArgs = func.getArgsNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());

	lst.clear();
	funcArgs.clear();

	lst.push_back("name_1");
	lst.push_back("name_2");
	lst.push_back("name_3");

	funcArgs.push_back("name_1");
	funcArgs.push_back("name_2");
	funcArgs.push_back("name_3");
	func.setArgsNamesFromList(funcArgs);
	funcArgs.clear();

	funcArgs = func.getArgsNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst.begin(), lst.end(), funcArgs.begin(), funcArgs.end());
}

BOOST_AUTO_TEST_SUITE_END();



/**
Тест argIsRef()
*/


BOOST_AUTO_TEST_CASE(testFunction_argsList)
{
	Function func;

	std::list<std::string> funcArgs;


	funcArgs.push_back("name_1");
	funcArgs.push_back("name_2");
	funcArgs.push_back("name_3");
	func.setArgsNamesFromList(funcArgs);
	func.addArgName("name_4");

	BOOST_CHECK_EQUAL(func.argIsRef("name_1"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_2"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_3"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_4"), false);

	func.argIsRef("name_1", true);
	func.argIsRef("name_2", true);
	func.argIsRef("name_3", true);
	func.argIsRef("name_4", true);

	BOOST_CHECK_EQUAL(func.argIsRef("name_1"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_2"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_3"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_4"), true);

	func.argIsRef("name_1", false);
	func.argIsRef("name_4", false);

	BOOST_CHECK_EQUAL(func.argIsRef("name_1"), false);
	BOOST_CHECK_EQUAL(func.argIsRef("name_2"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_3"), true);
	BOOST_CHECK_EQUAL(func.argIsRef("name_4"), false);


	BOOST_CHECK_THROW(func.argIsRef("name_5"), std::runtime_error);


}


/**
Тест копирования функции
*/


BOOST_AUTO_TEST_CASE(testFunction_copy)
{
	Function func;

	std::vector<Command> code;
	std::list<std::string> funcArgs;

	Value val(256, Value::SIGNED_INT, true, false);
	Array arr(10, Value::MOD8);

	arr[2] = Value(3, Value::MOD16, true, true);	

	arr.setWriteable(true);

	func.getDataKeeperPtr() -> addVar(val, "val");
	func.getDataKeeperPtr() -> addArray(arr, "arr");

	func.setName("function");

	funcArgs.push_back("name_1");
	funcArgs.push_back("name_2");
	funcArgs.push_back("name_3");
	func.setArgsNamesFromList(funcArgs);
	func.addArgName("name_4");
	func.argIsRef("name_1", true);
	func.argIsRef("name_4", true);


	Command c(Command::NOP);
	c.setLineNumber(1);

	code.push_back(c);
	c.setLineNumber(123);
	code.push_back(c);

	boost::shared_ptr<LabelOperand> plop = static_cast<boost::shared_ptr<LabelOperand> >(new LabelOperand("label_name"));
	plop -> setCommandOffset(10);

	c.setOperationType(Command::JMP);
	c.setFirstOperand(plop);
	code.push_back(c);

	boost::shared_ptr<ArrayOperand> parrop = static_cast<boost::shared_ptr<ArrayOperand> >(new ArrayOperand(&arr));
	boost::shared_ptr<VarOperand> pvarop = static_cast<boost::shared_ptr<VarOperand> >(new VarOperand(&val));

	parrop -> setArrayPtr(&(func.getDataKeeperPtr() -> getArray("arr")));
	pvarop -> setValuePtr(&(func.getDataKeeperPtr() -> getVarValue("val")));

	c.setOperationType(Command::RSZ);
	c.setFirstOperand(parrop);
	c.setSecondOperand(pvarop);
	code.push_back(c);


	pvarop = static_cast<boost::shared_ptr<VarOperand> >(new VarOperand(&val));
	pvarop -> setArrayElementPtr(&(func.getDataKeeperPtr() -> getArray("arr")), 2);
	c.setOperationType(Command::ADD);
	c.setFirstOperand(pvarop);
	pvarop = static_cast<boost::shared_ptr<VarOperand> >(new VarOperand(&val));
	pvarop -> setValuePtr(&(func.getDataKeeperPtr() -> getVarValue("val")));
	pvarop -> setType(Value::MOD8);
	c.setSecondOperand(pvarop);
	code.push_back(c);


	boost::shared_ptr<CallOperand> pcop = static_cast<boost::shared_ptr<CallOperand> >(new CallOperand);
	boost::shared_ptr<LabelOperand> plcop = static_cast<boost::shared_ptr<LabelOperand> >(new LabelOperand("label_name"));

	c = Command();
	c.setOperationType(Command::CALL);
	plcop -> setLabelName("call_name");
	c.setFirstOperand(plcop);
	pcop -> setArray(func.getDataKeeperPtr() -> getArray("arr"));
	c.setSecondOperand(pcop);

	pcop = static_cast<boost::shared_ptr<CallOperand> >(new CallOperand);
	pcop -> setValue(func.getDataKeeperPtr() -> getVarValue("val"));
	c.setOperand(3, pcop);

	code.push_back(c);


	func.setCommands(code);

	Function copyFunc = func.copy();

	func.getDataKeeperPtr() -> getArray("arr").resize(1);
	func.getDataKeeperPtr() -> getVarValue("val").setValue(10);
	func.getDataKeeperPtr() -> getVarValue("val").setReadable(false);
	func.getDataKeeperPtr() -> getVarValue("val").setWriteable(true);
	func.getDataKeeperPtr() -> getArray("arr").setWriteable(false);

	BOOST_CHECK_EQUAL(copyFunc.argIsRef("name_1"), true);
	BOOST_CHECK_EQUAL(copyFunc.argIsRef("name_2"), false);
	BOOST_CHECK_EQUAL(copyFunc.argIsRef("name_3"), false);
	BOOST_CHECK_EQUAL(copyFunc.argIsRef("name_4"), true);


	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> isExists("val"), true);
	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> isVar("val"), true);
	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> isArray("val"), false);

	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> isExists("arr"), true);
	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> isVar("arr"), false);
	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> isArray("arr"), true);

	BOOST_CHECK_EQUAL(copyFunc.getDataKeeperPtr() -> getNumberOfElements(), 2);


	BOOST_CHECK_THROW(copyFunc.argIsRef("name_5"), std::runtime_error);


	std::vector<Command> vect = copyFunc.getCommands();
	BOOST_CHECK_EQUAL_COLLECTIONS(vect.begin(), vect.end(), code.begin(), code.end());

	plop = boost::dynamic_pointer_cast<LabelOperand, Operand>(vect[2].getFirstOperand());

	BOOST_CHECK_EQUAL(plop -> getLabelName(), "label_name");
	BOOST_CHECK_EQUAL(plop -> getCommandOffset(), 10);

	parrop = boost::dynamic_pointer_cast<ArrayOperand, Operand>(vect[3].getFirstOperand());
	pvarop = boost::dynamic_pointer_cast<VarOperand, Operand>(vect[3].getSecondOperand());

	BOOST_CHECK_EQUAL(parrop -> getArrayPtr() -> size(), 10);
	BOOST_CHECK_EQUAL(parrop -> getArrayPtr() -> isWriteable(), true);
	BOOST_CHECK_EQUAL(parrop -> getArrayPtr() -> getType(), Value::MOD8);


	BOOST_CHECK_EQUAL(pvarop -> getValue(), 256);
	BOOST_CHECK_EQUAL(pvarop -> isWriteable(), false);
	BOOST_CHECK_EQUAL(pvarop -> isReadable(), true);

	pvarop = boost::dynamic_pointer_cast<VarOperand, Operand>(vect[4].getFirstOperand());
	BOOST_CHECK_EQUAL(pvarop -> getValue(), 3);
	pvarop = boost::dynamic_pointer_cast<VarOperand, Operand>(vect[4].getSecondOperand());
	BOOST_CHECK_EQUAL(pvarop -> getValue(), 0);

	plop = boost::dynamic_pointer_cast<LabelOperand, Operand>(vect[5].getFirstOperand());
	BOOST_CHECK_EQUAL(plop -> getLabelName(), "call_name");

	pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(vect[5].getSecondOperand());
	BOOST_CHECK_EQUAL(pcop -> getArray().size(), 10);

	pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(vect[5].getOperand(3));
	BOOST_CHECK_EQUAL(pcop -> getValue().getValue(), 256);
   

}
