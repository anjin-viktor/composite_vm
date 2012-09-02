#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "../src/Exception.h"


/**
@file ExceptionTest.cpp
@brief Набор модульных тестов для класса Exception
*/


BOOST_AUTO_TEST_SUITE(Exception_TEST_SUITE);


/**
Тест функции strToExceptionType
*/

BOOST_AUTO_TEST_CASE(Exception_TEST)
{
	using namespace Exception;
	BOOST_CHECK_EQUAL(strToExceptionType("numeric_error"), NumericError);
	BOOST_CHECK_EQUAL(strToExceptionType("constraint_error"), ConstraintError);
	BOOST_CHECK_EQUAL(strToExceptionType("incorrect"), NoType);


	BOOST_CHECK_EQUAL(exceptionTypeToStr(ConstraintError), "constraint_error");
	BOOST_CHECK_EQUAL(exceptionTypeToStr(NumericError), "numeric_error");
	BOOST_CHECK_EQUAL(exceptionTypeToStr(NoType), "no_type");
}



BOOST_AUTO_TEST_SUITE_END();
