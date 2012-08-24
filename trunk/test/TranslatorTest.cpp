#include <boost/test/unit_test.hpp>
#include "../src/Translator.h"
#include "Program.h"

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
	tr.translate();
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


	tr.translate();

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
	tr.translate();
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
	tr.translate();
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
Тест отказат трансляции некорректных входных данных (false_[n].mpr)
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
}


BOOST_AUTO_TEST_SUITE_END();