#include <stdexcept>


#include <boost/test/unit_test.hpp>
#include "../src/DataKeeper.h"
#include "../src/Value.h"
#include "../src/ParseError.h"



/**
@file DataKeeperTest.cpp
@brief Набор модульных тестов для класса DataKeeper.
*/


BOOST_AUTO_TEST_SUITE(DATAKEEPER_TEST_SUITE);


/**
Тест функций DataKeeper::addVar, DataKeeper::addArray.
*/


BOOST_AUTO_TEST_CASE(testDataKeeper_addVar_Arr)
{
	DataKeeper keeper;

	Value val1, val2, val3;
	Array arr1, arr2;

	keeper.addVar(val1, "name1");
	keeper.addVar(val2, "name2");

	BOOST_CHECK_THROW(keeper.addVar(val3, "name1"), std::runtime_error);

	keeper.addArray(arr1, "name3");
	BOOST_CHECK_THROW(keeper.addArray(arr2, "name2"), std::runtime_error);
	BOOST_CHECK_THROW(keeper.addArray(arr2, "name3"), std::runtime_error);
}



/**
Проверка функции isExists, isVar, isArr.
*/


BOOST_AUTO_TEST_CASE(testDataKeeper_isExists_isVar_isArray)
{
	DataKeeper keeper;
	BOOST_CHECK_EQUAL(keeper.isExists("name"), false);
	BOOST_CHECK_EQUAL(keeper.isVar("name"), false);
	BOOST_CHECK_EQUAL(keeper.isArray("name"), false);

	keeper.addVar(Value(), "name");
	BOOST_CHECK_EQUAL(keeper.isExists("name"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("name"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("name"), false);

	keeper.addVar(Value(), "name1");
	BOOST_CHECK_EQUAL(keeper.isExists("name1"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("name1"), true);
	BOOST_CHECK_EQUAL(keeper.isArray("name1"), false);

	BOOST_CHECK_THROW(keeper.addVar(Value(), "name"), std::runtime_error);
	BOOST_CHECK_THROW(keeper.addArray(Array(), "name"), std::runtime_error);

	keeper.addArray(Array(), "name2");
	BOOST_CHECK_EQUAL(keeper.isExists("name2"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("name2"), false);
	BOOST_CHECK_EQUAL(keeper.isArray("name2"), true);


}


/**
Проверка функций add/getVar/Array.
*/
BOOST_AUTO_TEST_CASE(testDataKeeper_add_get_Var_Array)
{
	DataKeeper keeper;
	keeper.addVar(Value(1), "val_name");

	keeper.getVarValue("val_name") = Value(2);
	BOOST_CHECK_EQUAL(keeper.getVarValue("val_name").getValue(), 2);
	keeper.getVarValue("val_name").setValue(3);
	BOOST_CHECK_EQUAL(keeper.getVarValue("val_name").getValue(), 3);


	keeper.getVarValue("val_name");

	Array arr(2);
	arr[0] = 1;

	keeper.addArray(arr, "arr_name");
	keeper.getArray("arr_name")[0].setValue(2);
	BOOST_CHECK_EQUAL(keeper.getArray("arr_name")[0].getValue(), 2);

	keeper.getArray("arr_name");

	BOOST_CHECK_THROW(keeper.getArray("not_exists_arr"), std::runtime_error);
	BOOST_CHECK_THROW(keeper.getVarValue("not_exists_var"), std::runtime_error);
	BOOST_CHECK_THROW(keeper.getVarValue("arr_name"), std::runtime_error);
	BOOST_CHECK_THROW(keeper.getArray("val_name"), std::runtime_error);
}




/**
Проверка функции clear.
*/
BOOST_AUTO_TEST_CASE(testDataKeeper_clear)
{
	DataKeeper keeper;
	keeper.addVar(Value(1), "val_name");
	keeper.addArray(Array(), "arr_name");

	BOOST_CHECK_EQUAL(keeper.isExists("val_name"), true);
	BOOST_CHECK_EQUAL(keeper.isExists("arr_name"), true);

	keeper.clear();

	BOOST_CHECK_EQUAL(keeper.isExists("val_name"), false);
	BOOST_CHECK_EQUAL(keeper.isExists("arr_name"), false);
}






BOOST_AUTO_TEST_SUITE_END();
