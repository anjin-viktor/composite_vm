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

	BOOST_CHECK_EQUAL(stream.str(), "hello!\n");
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

	Function func = Program::getInstance().getFunction("main").copy();
	pkeeper = func.getDataKeeperPtr();


	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isReadable(), true);
	BOOST_CHECK_EQUAL(pkeeper -> getVarValue("ch").isWriteable(), true);


	BOOST_CHECK_NO_THROW(exec.exec());
	BOOST_CHECK_EQUAL(stream.str(), "Hello!\n"
									"Rello!\n"
									"Hello!\n"
									"hello!\n"
									"hello!\n"
	);
}


/**
Тест выполнения 3.mpr(сравнение, условные переходы)
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_3)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/3.mpr");

	std::ostringstream stream;
	exec.setOutputStream(stream);

	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_NO_THROW(exec.exec());
	BOOST_CHECK_EQUAL(stream.str(), "l2\n"
									"l1\n"
									"l3\n"
									"l5\n"
									"l4\n"
									"l6\n"
									"l7\n"
						);
}



/**
Тест выполнения 4.mpr(команда rsz)
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_4)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/4.mpr");

	std::ostringstream stream;
	std::ostringstream err;

	exec.setOutputStream(stream);
	exec.setErrorStream(err);

	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_NO_THROW(exec.exec());

	BOOST_CHECK_EQUAL(stream.str(), "2\n");
	BOOST_CHECK_EQUAL(err.str(), "Program is interrupted with exception `constraint_error`\n");
}




/**
Тест выполнения 5.mpr(команды call, ret)
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_5)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/5.mpr");

	std::ostringstream stream;
	exec.setOutputStream(stream);

	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_NO_THROW(exec.exec());

	BOOST_CHECK_EQUAL(stream.str(), "main\n"
									"f\n"
									"from f\n"
									"FROm f\n"
									"FROM f\n"
									"FROm f\n"
									"FROM f\n"
									"FROM f\n"
									"FROM f\n"
									"FROM F\n"
									"main\n"
						);

}


/**
Тест на обработчики исключений (6.mpr)
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_6)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/6.mpr");

	std::ostringstream stream;
	std::ostringstream err;

	exec.setOutputStream(stream);
	exec.setErrorStream(err);

	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_NO_THROW(exec.exec());

	BOOST_CHECK_EQUAL(stream.str(), "program\n"
									"f3\n"
									"numeric\n"
						);
	BOOST_CHECK_EQUAL(err.str(), "Program is interrupted with exception `numeric_error`\n");
}

/**
Тест корректной работы
*/

BOOST_AUTO_TEST_CASE(CodeExecuter_7)
{
	Translator tr;
	CodeExecuter exec;

	std::ostringstream stream;
	std::ostringstream err;

	exec.setOutputStream(stream);
	exec.setErrorStream(err);

	tr.setInputFileName("CodeExecuterTestFiles/7.mpr");
	BOOST_CHECK_NO_THROW(tr.translate());
	BOOST_CHECK_NO_THROW(exec.exec());
	BOOST_CHECK_EQUAL(err.str(), "");
	BOOST_CHECK_EQUAL(stream.str(), "constraint error\n");
}


/**
Тест выполнения 8.mpr(команды сдвига)
*/
BOOST_AUTO_TEST_CASE(CodeExecuter_8)
{
	Translator tr;
	CodeExecuter exec;

	tr.setInputFileName("CodeExecuterTestFiles/8.mpr");

	std::ostringstream stream;
	std::ostringstream err;

	exec.setOutputStream(stream);
	exec.setErrorStream(err);

	BOOST_CHECK_NO_THROW(tr.translate());

	BOOST_CHECK_NO_THROW(exec.exec());

	BOOST_CHECK_EQUAL(stream.str(), "hello, world!!!\n"
									"Hello, world!!!\n"
									"HEllo, world!!!\n"
									"msg\n"
									"numeric error\n"
									"msg\n"
		);
	BOOST_CHECK_EQUAL(err.str(), "Program is interrupted with exception `numeric_error`\n");
}



BOOST_AUTO_TEST_SUITE_END();
