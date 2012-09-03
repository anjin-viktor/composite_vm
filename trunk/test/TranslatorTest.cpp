#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "../src/Translator.h"
#include "Program.h"
#include "VarOperand.h"
#include "CallOperand.h"
#include "LabelOperand.h"
#include "ArrayOperand.h"

/**
@file TranslatorTest.cpp
@brief Набор модульных тестов для класса Translator
*/


BOOST_AUTO_TEST_SUITE(Translator_TEST_SUITE);



/**
Тест конструктора и функций set/getInputFileName
*/
BOOST_AUTO_TEST_CASE(TranslatorSetGet_Test)
{
	Translator tr1, tr2("test_name");
	tr1.setInputFileName("test_name_");

	BOOST_CHECK_EQUAL(tr1.getInputFileName(), "test_name_");
	BOOST_CHECK_EQUAL(tr2.getInputFileName(), "test_name");
}



/**
Тест правильности трансляции 1.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_1_Test)
{
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/1.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);

	std::vector<Command> v1, v2;
	Command c;
	c.setOperationType(Command::NOP);
	c.setLineNumber(4);

	v1.push_back(c);
	v2 = Program::getInstance().getFunction("main").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 0);
}


/**
Тест правильности трансляции 2.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_2_Test)
{
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/2.mpr");


	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);


	std::vector<Command> v1, v2;
	Command c;

	c.setOperationType(Command::MOV);
	c.setLineNumber(10);
	v1.push_back(c);


	c.setOperationType(Command::ADD);
	c.setLineNumber(11);
	v1.push_back(c);

	c.setOperationType(Command::DIV);
	c.setLineNumber(12);
	v1.push_back(c);

	c.setOperationType(Command::JMP);
	c.setLineNumber(17);
	v1.push_back(c);

	c.setOperationType(Command::CMP);
	c.setLineNumber(20);
	v1.push_back(c);

	c.setOperationType(Command::JE);
	c.setLineNumber(21);
	v1.push_back(c);

	c.setOperationType(Command::JMP);
	c.setLineNumber(24);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 4);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> isExists("n"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> isExists("ch"), true);

}



/**
Тест правильности трансляции 3.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_3_Test)
{
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/3.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);

	std::vector<Command> v1, v2;
	Command c;
	c.setOperationType(Command::MOV);
	c.setLineNumber(7);
	v1.push_back(c);

	c.setOperationType(Command::ADD);
	c.setLineNumber(8);
	v1.push_back(c);

	c.setOperationType(Command::MUL);
	c.setLineNumber(9);
	v1.push_back(c);

	c.setOperationType(Command::MUL);
	c.setLineNumber(10);
	v1.push_back(c);

	c.setOperationType(Command::RSZ);
	c.setLineNumber(11);
	v1.push_back(c);

	c.setOperationType(Command::AOUT);
	c.setLineNumber(12);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 3);
}




/**
Тест правильности трансляции 4.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_4_Test)
{
	std::string callName;
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/4.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 3);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f1"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f2"), true);

	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::CALL);
	c.setLineNumber(4);

	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(callName, "f1");

	v1[0].setLineNumber(12);
	v2 = Program::getInstance().getFunction("f1").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(callName, "f2");

	v1[0].setLineNumber(20);
	v2 = Program::getInstance().getFunction("f2").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(callName, "main");


	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 0);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f1").getDataKeeperPtr() -> getNumberOfElements(), 0);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f2").getDataKeeperPtr() -> getNumberOfElements(), 0);
}





/**
Тест правильности трансляции 5.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_5_Test)
{
	std::string callName;
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/5.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 3);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("func1"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("func2"), true);

	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::CALL);
	c.setLineNumber(14);

	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(callName, "func2");

	c.setOperationType(Command::NOP);
	c.setLineNumber(5);
	v1[0].setLineNumber(6);
	v1.push_back(c);

	c = v1[0];
	v1[0] = v1[1];
	v1[1] = c;

	v2 = Program::getInstance().getFunction("func2").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[1].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(callName, "func1");

	v1.clear();
	c.setOperationType(Command::NOP);
	c.setLineNumber(23);
	v1.push_back(c);
	v2 = Program::getInstance().getFunction("func1").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("func2").getDataKeeperPtr() -> getNumberOfElements(), 2);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("func1").getDataKeeperPtr() -> getNumberOfElements(), 3);
}




/**
Тест правильности трансляции 6.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_6_Test)
{
	std::string callName;
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/6.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 2);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f"), true);

	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 2);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f").getDataKeeperPtr() -> getNumberOfElements(), 3);


	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::CALL);
	c.setLineNumber(13);

	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(callName, "f");

	c.setOperationType(Command::NOP);
	c.setLineNumber(4);
	v1[0] = c;

	v2 = Program::getInstance().getFunction("f").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
}





/**
Тест правильности трансляции 7.mpr и проверка работы с параметрами модуля при их 
передаче по значению (не по ссылке)
*/
BOOST_AUTO_TEST_CASE(Translator_7_Test)
{
	std::string callName;
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/7.mpr");

	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 2);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("function"), true);

	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 3);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("function").getDataKeeperPtr() -> getNumberOfElements(), 4);


	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::CALL);
	c.setLineNumber(13);

	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(v2[0].getNumberOfOperands(), 5);
	BOOST_CHECK_EQUAL(callName, "function");

	c.setOperationType(Command::NOP);
		c.setLineNumber(4);
	v1[0] = c;

	v2 = Program::getInstance().getFunction("function").getCommands();
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	DataKeeper *pkeeper = Program::getInstance().getFunction("function").getDataKeeperPtr();

	BOOST_CHECK_EQUAL(pkeeper -> isVar("c"), true);
	BOOST_CHECK_EQUAL(pkeeper -> isVar("k"), true);
	BOOST_CHECK_EQUAL(pkeeper -> isVar("t"), true);
	BOOST_CHECK_EQUAL(pkeeper -> isVar("m"), true);

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("c").getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("k").getType(), Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("t").getType(), Value::MOD16);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("m").getType(), Value::MOD32);

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("c").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("c").isWriteable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("k").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("k").isWriteable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("t").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("t").isWriteable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("m").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("m").isWriteable(), true);


	pkeeper -> getVarValue("c").setValue(5);
	pkeeper -> getVarValue("k").setValue(5);
	pkeeper -> getVarValue("t").setValue(5);
	pkeeper -> getVarValue("m").setValue(5);

	pkeeper = Program::getInstance().getFunction("main").getDataKeeperPtr();

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").getValue(), 1);
	BOOST_CHECK_EQUAL(pkeeper -> getArray("arr")[3].getValue(), 4);
}






/**
Тест правильности трансляции 8.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_8_Test)
{
	std::string callName;
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/8.mpr");


	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);

	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 2);


	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::NOP);
	c.setLineNumber(5);

	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	c.setOperationType(Command::MOV);
	c.setLineNumber(8);
	v1[0] = c;

	v2 = Program::getInstance().getFunction("main").getExceptionHandlerCode(Exception::NumericError);
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	c.setOperationType(Command::ADD);
	c.setLineNumber(11);
	v1[0] = c;

	v2 = Program::getInstance().getFunction("main").getExceptionHandlerCode(Exception::ConstraintError);
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
}


/**
Тест правильности трансляции 9.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_9_Test)
{
	std::string callName;
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/9.mpr");

	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 3);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("function"), true);

	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 3);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f").getDataKeeperPtr() -> getNumberOfElements(), 3);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("function").getDataKeeperPtr() -> getNumberOfElements(), 4);


	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::NOP);
	c.setLineNumber(15);

	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());



	c.setOperationType(Command::JMP);
	c.setLineNumber(19);
	v1[0] = c;

	c.setOperationType(Command::CALL);
	c.setLineNumber(20);
	v1.push_back(c);

	c.setOperationType(Command::JE);
	c.setLineNumber(21);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getExceptionHandlerCode(Exception::NumericError);
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[1].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL(callName, "f");
	BOOST_CHECK_EQUAL(v2.size(), v1.size());

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	v1.clear();
	c.setOperationType(Command::CALL);
	c.setLineNumber(26);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getExceptionHandlerCode(Exception::ConstraintError);
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();

	BOOST_CHECK_EQUAL(callName, "function");
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
}





/**
Тест трансляции 10.mpr без исключений
*/
BOOST_AUTO_TEST_CASE(Translator_10_Test)
{
	Translator tr;
	tr.setInputFileName("TranslatorTestFiles/10.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 5);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f1"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f2"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f3"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f4"), true);

}




/**
Тест трансляции 11.mpr
*/
BOOST_AUTO_TEST_CASE(Translator_11_Test)
{
	Translator tr;
	std::string callName, labelName;

	tr.setInputFileName("TranslatorTestFiles/11.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_EQUAL(Program::getInstance().numberOfFunctions(), 5);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("main"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f1"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f2"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f3"), true);
	BOOST_CHECK_EQUAL(Program::getInstance().functionIsExists("f4"), true);



	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("main").getDataKeeperPtr() -> getNumberOfElements(), 5);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f1").getDataKeeperPtr() -> getNumberOfElements(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f2").getDataKeeperPtr() -> getNumberOfElements(), 1);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f3").getDataKeeperPtr() -> getNumberOfElements(), 2);
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f4").getDataKeeperPtr() -> getNumberOfElements(), 2);


	std::vector<Command> v1, v2;

	Command c;
	c.setOperationType(Command::CALL);
	c.setLineNumber(21);
	v1.push_back(c);

	c.setLineNumber(22);
	v1.push_back(c);

	c.setLineNumber(23);
	v1.push_back(c);

	c.setLineNumber(24);
	v1.push_back(c);

	c.setOperationType(Command::MOV);
	c.setLineNumber(26);
	v1.push_back(c);

	c.setOperationType(Command::MUL);
	c.setLineNumber(27);
	v1.push_back(c);

	c.setOperationType(Command::JMP);
	c.setLineNumber(28);
	v1.push_back(c);

	c.setOperationType(Command::ADD);
	c.setLineNumber(29);
	v1.push_back(c);

	c.setOperationType(Command::JMP);
	c.setLineNumber(32);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("main").getCommands();
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[0].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(callName, "f1");
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[1].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(callName, "f2");
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[2].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(callName, "f3");
	callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[3].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(callName, "f4");
	boost::shared_ptr<CallOperand> pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(v2[0].getOperand(1));
	BOOST_CHECK_EQUAL(pcop -> isArray(), true);
	pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(v2[1].getOperand(1));
	BOOST_CHECK_EQUAL(pcop -> isValue(), true);
	pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(v2[2].getOperand(1));
	BOOST_CHECK_EQUAL(pcop -> isArray(), true);
	pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(v2[3].getOperand(1));
	BOOST_CHECK_EQUAL(pcop -> isValue(), true);

	boost::shared_ptr<VarOperand> pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[4].getOperand(0));
	BOOST_CHECK_EQUAL(pvop -> hasValue(), true);
	BOOST_CHECK_EQUAL(pvop -> getValue(), 100);
	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[4].getOperand(1));
	BOOST_CHECK_EQUAL(pvop -> hasValue(), true);
	BOOST_CHECK_EQUAL(pvop -> getValue(), 150);

	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[5].getOperand(0));
	BOOST_CHECK_EQUAL(pvop -> getValue(), 100);
	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[5].getOperand(1));
	BOOST_CHECK_EQUAL(pvop -> getValue(), 100);

	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[7].getFirstOperand());
	BOOST_CHECK_EQUAL(pvop -> getValue(), 1);
	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[7].getFirstOperand());
	BOOST_CHECK_EQUAL(pvop -> getValue(), 1);

	labelName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[6].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(labelName, "label");
	labelName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[8].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(labelName, "start");


	v1.clear();
	c.setOperationType(Command::ADD);
	c.setLineNumber(35);
	v1.push_back(c);

	v2 = Program::getInstance().getFunction("main").getExceptionHandlerCode(Exception::NumericError);
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());

	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[0].getOperand(0));
	BOOST_CHECK_EQUAL(pvop -> getValue(), 0);
	pvop = boost::dynamic_pointer_cast<VarOperand, Operand>(v2[0].getSecondOperand());
	BOOST_CHECK_EQUAL(pvop -> getValue(), 2);


	v1.clear();
	c.setOperationType(Command::NOP);
	c.setLineNumber(6);
	v1.push_back(c);

	c.setOperationType(Command::JMP);
	c.setLineNumber(7);
	v1.push_back(c);

	v2 = Program::getInstance().getFunction("f1").getCommands();

	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	labelName = boost::dynamic_pointer_cast<LabelOperand, Operand>(v2[1].getFirstOperand()) -> getLabelName();
	BOOST_CHECK_EQUAL(labelName, "f1_label");
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f1").getDataKeeperPtr() -> getArray("arr").isWriteable(), false);

	v1.clear();
	c.setOperationType(Command::NOP);
	c.setLineNumber(43);
	v1.push_back(c);

	v2 = Program::getInstance().getFunction("f2").getCommands();
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f2").getDataKeeperPtr() -> getVarValue("n").isWriteable(), false);


	v1.clear();
	c.setOperationType(Command::NOP);
	c.setLineNumber(52);
	v1.push_back(c);

	c.setOperationType(Command::MOV);
	c.setLineNumber(53);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("f3").getCommands();
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f3").getDataKeeperPtr() -> getArray("arr").isWriteable(), false);


	v1.clear();
	c.setOperationType(Command::NOP);
	c.setLineNumber(61);
	v1.push_back(c);
	c.setOperationType(Command::RSZ);
	c.setLineNumber(62);
	v1.push_back(c);


	v2 = Program::getInstance().getFunction("f4").getCommands();
	BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), v2.begin(), v2.end());
	BOOST_CHECK_EQUAL(Program::getInstance().getFunction("f4").getDataKeeperPtr() -> getVarValue("n").isWriteable(), false);
}



/**
Тест отказа трансляции некорректных входных данных (false_[n].mpr)
*/
BOOST_AUTO_TEST_CASE(Translator_false_Test)
{
	Translator tr;

	tr.setInputFileName("TranslatorTestFiles/false_1.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_2.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_3.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_4.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_5.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_6.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_7.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_8.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_9.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_10.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_11.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_12.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_13.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_14.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_15.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_16.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_17.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_18.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_19.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_20.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);

	tr.setInputFileName("TranslatorTestFiles/false_21.mpr");
	BOOST_CHECK_THROW(tr.translate(), ParseError);
}


BOOST_AUTO_TEST_SUITE_END();