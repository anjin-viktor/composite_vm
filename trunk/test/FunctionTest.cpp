#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../src/Function.h"
#include "../src/Command.h"
#include "../src/Value.h"
#include "../src/Array.h"
#include "../src/LabelOperand.h"
#include "../src/ArrayOperand.h"
#include "../src/VarOperand.h"
#include "../src/CallOperand.h"
#include "../src/Command.h"
#include "../src/Translator.h"
#include "../src/DataKeeper.h"

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



/**
Тест argIsRef()
*/


BOOST_AUTO_TEST_CASE(testFunction_argIsRef)
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
	pcop -> setArrayPtr(&(func.getDataKeeperPtr() -> getArray("arr")));
	c.setSecondOperand(pcop);

	pcop = static_cast<boost::shared_ptr<CallOperand> >(new CallOperand);
	pcop -> setValuePtr(&(func.getDataKeeperPtr() -> getVarValue("val")));
	c.setOperand(3, pcop);

	code.push_back(c);

	plcop = static_cast<boost::shared_ptr<LabelOperand> >(new LabelOperand("label_name"));
	pcop = static_cast<boost::shared_ptr<CallOperand> >(new CallOperand);
	pcop -> setArrayElement(&(func.getDataKeeperPtr() -> getArray("arr")), 2);


	c = Command();
	c.setOperationType(Command::CALL);
	c.setFirstOperand(plcop);
	c.setSecondOperand(pcop);
	code.push_back(c);


	func.setCommands(code);

	Function copyFunc = func.copy();


	func.getDataKeeperPtr() -> getArray("arr").resize(25);
	func.getDataKeeperPtr() -> getVarValue("val").setValue(10);
	func.getDataKeeperPtr() -> getVarValue("val").setReadable(false);
	func.getDataKeeperPtr() -> getVarValue("val").setWriteable(true);
	func.getDataKeeperPtr() -> getArray("arr").setWriteable(false);
	(func.getDataKeeperPtr() -> getArray("arr"))[2].setValue(1);


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


	plop = boost::dynamic_pointer_cast<LabelOperand, Operand>(vect[6].getFirstOperand());
	pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(vect[6].getSecondOperand());
	BOOST_CHECK_EQUAL(plop -> getLabelName(), "label_name");
	BOOST_CHECK_EQUAL(pcop -> isArrayElement(), true);
}


/**
Тест прав доступа переменных при копировании функции
*/


BOOST_AUTO_TEST_CASE(testFunction_copy_access)
{
	Function func;
	std::vector<Command> code;
	Command c;

	Value val(256, Value::SIGNED_INT, true, true);
	Value val1(0, Value::SIGNED_INT, true, false);
	Array arr(3, Value::MOD8);


	arr[0] = Value(3, Value::MOD16, false, false);
	arr[1] = Value(3, Value::MOD16, false, true);
	arr[2] = Value(3, Value::MOD16, true, true);
	arr.setWriteable(true);

	func.getDataKeeperPtr() -> addVar(val, "val");
	func.getDataKeeperPtr() -> addVar(val1, "0");
	func.getDataKeeperPtr() -> addArray(arr, "arr");

	c.setOperationType(Command::MOV);
	boost::shared_ptr<VarOperand> pop1 = boost::shared_ptr<VarOperand>(new VarOperand(&(func.getDataKeeperPtr() -> getVarValue("val"))));
	boost::shared_ptr<VarOperand> pop2 = boost::shared_ptr<VarOperand>(new VarOperand(&(func.getDataKeeperPtr() ->  getArray("arr")),2, Value::NO_TYPE));

	pop2 -> setType(Value::SIGNED_INT);
	c.setFirstOperand(pop1);
	c.setSecondOperand(pop2);
	code.push_back(c);

	func.setCommands(code);

	code = func.getCommands();
	c = code[0];

	Function copyFunc = func.copy();

	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getVarValue("val")).isWriteable(), true);
	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getVarValue("val")).isReadable(), true);

	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getArray("arr"))[0].isReadable(), false);
	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getArray("arr"))[0].isWriteable(), false);
	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getArray("arr"))[1].isReadable(), false);
	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getArray("arr"))[1].isWriteable(), true);
	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getArray("arr"))[2].isReadable(), true);
	BOOST_CHECK_EQUAL((copyFunc.getDataKeeperPtr() -> getArray("arr"))[2].isWriteable(), true);

	code = copyFunc.getCommands();
	c = code[0];

	boost::shared_ptr<VarOperand> pop = boost::dynamic_pointer_cast<VarOperand, Operand>(c.getFirstOperand());
	BOOST_CHECK_EQUAL(pop -> isWriteable(), true);
	BOOST_CHECK_EQUAL(pop -> isReadable(), true);

	pop = boost::dynamic_pointer_cast<VarOperand, Operand>(c.getSecondOperand());
	BOOST_CHECK_EQUAL(pop -> hasValue(), true);
	BOOST_CHECK_EQUAL(pop -> isWriteable(), false);
	BOOST_CHECK_EQUAL(pop -> isReadable(), true);
}




/**
Тест 1.mpr
*/

BOOST_AUTO_TEST_CASE(testFunction_1)
{
	Translator tr;

	tr.setInputFileName("FunctionTestFiles/1.mpr");


	BOOST_CHECK_NO_THROW(tr.translate());

	DataKeeper *pkeeper = Program::getInstance().getFunction("main").getDataKeeperPtr();

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);

	Function f = Program::getInstance().getFunction("main").copy();
	Function func = f;
	pkeeper = func.getDataKeeperPtr();

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").getValue(), 70);

	std::vector<Command> code = func.getCommands();
	BOOST_CHECK_EQUAL(code[0].getOperationType(), Command::MOV);

	boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand> (code[0].getFirstOperand());
	boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand> (code[0].getSecondOperand());


	BOOST_CHECK_EQUAL(pfArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isWriteable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isConstant(), false);
	BOOST_CHECK_EQUAL(pfArg -> isCast(), false);
	BOOST_CHECK_EQUAL(pfArg -> getValue(), 70);

	BOOST_CHECK_EQUAL(psArg -> isWriteable(), false);
	BOOST_CHECK_EQUAL(psArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(psArg -> getValue(), 72);

}



/**
Тест копирования функции
*/
BOOST_AUTO_TEST_CASE(testFunction_1_)
{
	Translator tr;

	tr.setInputFileName("FunctionTestFiles/1.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	Function f, func;
	func = Program::getInstance().getFunction("___main____").copy();

	Program::getInstance().getFunction("___main____");

	f = func;
	func = f;

	std::vector<Command> code;
	code = func.getCommands();


	boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
		(code[0].getFirstOperand());
	boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
		(code[0].getSecondOperand());


	BOOST_CHECK_EQUAL(pfArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isWriteable(), true);
	BOOST_CHECK_EQUAL(pfArg -> isConstant(), false);
	BOOST_CHECK_EQUAL(pfArg -> isCast(), false);
	BOOST_CHECK_EQUAL(pfArg -> getValue(), 70);

	BOOST_CHECK_EQUAL(psArg -> isWriteable(), false);
	BOOST_CHECK_EQUAL(psArg -> isReadable(), true);
	BOOST_CHECK_EQUAL(psArg -> getValue(), 72);

}


/**
Тест копирования функции (2.mpr)
*/
BOOST_AUTO_TEST_CASE(testFunction_2_)
{
	Translator tr;

	tr.setInputFileName("FunctionTestFiles/2.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	Function f, func;
	func = Program::getInstance().getFunction("f").copy();


	f = func;
	func = f;

	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> isArray("str"), true);

	std::vector<Command> code;
	code = func.getCommands();


	boost::shared_ptr<CallOperand> parg = boost::dynamic_pointer_cast<CallOperand, Operand>
		(code[0].getSecondOperand());

/*	BOOST_CHECK_EQUAL(parg -> isArray(), true);
	BOOST_CHECK_EQUAL(parg -> isValue(), false);
*/
	BOOST_CHECK(parg -> isArray() == true);
	BOOST_CHECK(parg -> isValue() == false);

}




/**
Тест копирования функции (3.mpr)
*/
BOOST_AUTO_TEST_CASE(testFunction_3_)
{
	Translator tr;

	tr.setInputFileName("FunctionTestFiles/3.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	Function func, f_;
	func = Program::getInstance().getFunction("f1").copy();


	Function f(func);
	f_ = f;

	BOOST_CHECK_EQUAL(f_.getDataKeeperPtr() -> isArray("str"), true);

	std::vector<Command> code;
	code = f_.getCommands();


	boost::shared_ptr<CallOperand> parg = boost::dynamic_pointer_cast<CallOperand, Operand>
		(code[4].getSecondOperand());

	BOOST_CHECK_EQUAL(parg -> isArray(), true);
	BOOST_CHECK_EQUAL(parg -> isValue(), false);
}



/**
Тест копирования функции (4.mpr)
*/
BOOST_AUTO_TEST_CASE(testFunction_4_)
{
/*
	ссылки - str и ch
*/
	Translator tr;

	tr.setInputFileName("FunctionTestFiles/4.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	Function func;
	func = Program::getInstance().getFunction("f").copy();


	func.getDataKeeperPtr() -> getVarValue("ch").setValue(1);
	func.getDataKeeperPtr() -> getVarValue("ch_").setValue(2);

	func.getDataKeeperPtr() -> getArray("str").resize(1);
	func.getDataKeeperPtr() -> getArray("str")[0].setValue(1);

	func.getDataKeeperPtr() -> getArray("str_").resize(1);
	func.getDataKeeperPtr() -> getArray("str_")[0].setValue(2);


	Function f;
	f = func;

	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getVarValue("ch").getValue(), 1);
	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getVarValue("ch_").getValue(), 2);
	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getArray("str")[0].getValue(), 1);
	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getArray("str_")[0].getValue(), 2);

	f.getDataKeeperPtr() -> getVarValue("ch").setValue(3);
	f.getDataKeeperPtr() -> getVarValue("ch_").setValue(4);
	f.getDataKeeperPtr() -> getArray("str")[0].setValue(5);
	f.getDataKeeperPtr() -> getArray("str_")[0].setValue(6);

	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getVarValue("ch").getValue(), 3);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getVarValue("ch_").getValue(), 2);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getArray("str")[0].getValue(), 5);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getArray("str_")[0].getValue(), 2);

	Function f1(func);

	BOOST_CHECK_EQUAL(f1.getDataKeeperPtr() -> getVarValue("ch").getValue(), 3);
	BOOST_CHECK_EQUAL(f1.getDataKeeperPtr() -> getVarValue("ch_").getValue(), 2);
	BOOST_CHECK_EQUAL(f1.getDataKeeperPtr() -> getArray("str")[0].getValue(), 5);
	BOOST_CHECK_EQUAL(f1.getDataKeeperPtr() -> getArray("str_")[0].getValue(), 2);

	f1.getDataKeeperPtr() -> getVarValue("ch").setValue(1);
	f1.getDataKeeperPtr() -> getVarValue("ch_").setValue(6);
	f1.getDataKeeperPtr() -> getArray("str")[0].setValue(1);
	f1.getDataKeeperPtr() -> getArray("str_")[0].setValue(7);

	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getVarValue("ch").getValue(), 1);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getVarValue("ch_").getValue(), 2);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getArray("str")[0].getValue(), 1);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getArray("str_")[0].getValue(), 2);

	f = func.copy();

	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getVarValue("ch").getValue(), 1);
	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getVarValue("ch_").getValue(), 2);
	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getArray("str")[0].getValue(), 1);
	BOOST_CHECK_EQUAL(f.getDataKeeperPtr() -> getArray("str_")[0].getValue(), 2);

	f.getDataKeeperPtr() -> getVarValue("ch").setValue(3);
	f.getDataKeeperPtr() -> getVarValue("ch_").setValue(4);
	f.getDataKeeperPtr() -> getArray("str")[0].setValue(5);
	f.getDataKeeperPtr() -> getArray("str_")[0].setValue(6);

	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getVarValue("ch").getValue(), 1);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getVarValue("ch_").getValue(), 2);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getArray("str")[0].getValue(), 1);
	BOOST_CHECK_EQUAL(func.getDataKeeperPtr() -> getArray("str_")[0].getValue(), 2);

}





BOOST_AUTO_TEST_SUITE_END();
