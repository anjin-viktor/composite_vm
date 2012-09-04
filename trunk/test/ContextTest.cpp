#include <vector>
#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "../src/Context.h"
#include "../src/Command.h"


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


BOOST_AUTO_TEST_SUITE_END();
