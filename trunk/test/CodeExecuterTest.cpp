#include <stdexcept>
#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "../src/CodeExecuter.h"
#include "../src/Translator.h"


/**
@file CodeExecuterTest.cpp
@brief Набор модульных тестов для класса CodeExecuter
*/


BOOST_AUTO_TEST_SUITE(CodeExecuter_TEST_SUITE);


/**
Тест #1
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_0)
{
	CodeExecuter exec;
	BOOST_CHECK_THROW(exec.exec(), std::runtime_error)
}



/**
Тест выполнения 1.mpr
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_1)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/1.mpr");

	std::ostringstream stream;
	exec.setOutputStream(stream);

	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_NO_THROW(exec.exec());

	BOOST_CHECK_EQUAL(stream.str(), "hello!");
}


/**
Тест выполнения 2.mpr(арифметические операции)
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_2)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/2.mpr");

	std::ostringstream stream;
	exec.setOutputStream(stream);

	BOOST_CHECK_NO_THROW(tr.translate());

	DataKeeper *pkeeper = Program::getInstance().getFunction("main").getDataKeeperPtr();

	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);

//	BOOST_CHECK_NO_THROW(exec.exec());


//	BOOST_CHECK_EQUAL(stream.str(), "Hello!");
}

BOOST_AUTO_TEST_SUITE_END();
