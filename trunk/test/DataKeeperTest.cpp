#include <stdexcept>


#include <boost/test/unit_test.hpp>
#include "../src/DataKeeper.h"
#include "../src/Value.h"
#include "../src/ParseError.h"



/**
@file DataKeeperTest.cpp
@brief Набор модульных тестов для класса DataKeeper
*/


BOOST_AUTO_TEST_SUITE(DATAKEEPER_TEST_SUITE);


/**
Тест функций DataKeeper::addVar, DataKeeper::addArray
*/


BOOST_AUTO_TEST_CASE(testDataKeeper_addVar_Arr)
{
	DataKeeper keeper;

	Value val1, val2, val3;
	Array arr1, arr2;

	keeper.addVar(val1, "name1");
	keeper.addVar(val2, "name2");

	BOOST_CHECK_THROW(keeper.addVar(val3, "name1"), ParseError);

	keeper.addArray(arr1, "name3");
	BOOST_CHECK_THROW(keeper.addArray(arr2, "name2"), ParseError);
}


BOOST_AUTO_TEST_SUITE_END();