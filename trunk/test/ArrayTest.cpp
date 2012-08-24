#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "../src/Array.h"


/**
@file ArrayTest.cpp
@brief Набор модульных тестов для класса Array
*/


BOOST_AUTO_TEST_SUITE(Array_TEST_SUITE);



BOOST_AUTO_TEST_CASE(ArrayTest_Constructor_0)
{
	Array arr;
	BOOST_CHECK_EQUAL(arr.size(), 0);
	BOOST_CHECK_EQUAL(arr.isWriteable(), true);

	arr.resize(1000);
	arr.setType(Value::MOD8);
	for(int i=0; i<arr.size(); i++)
		arr[i].setWriteable(true);

	BOOST_CHECK_EQUAL(arr[999].getType(), Value::MOD8);
	BOOST_CHECK_EQUAL(arr[500].isReadable(), false);
	BOOST_CHECK_EQUAL(arr[500].isWriteable(), true);

	arr.setWriteableAll(false);
	arr.setReadableAll(true);
	BOOST_CHECK_EQUAL(arr[500].isReadable(), true);
	BOOST_CHECK_EQUAL(arr[500].isWriteable(), false);

	arr.resize(0);
	arr.resize(5);
	BOOST_CHECK_EQUAL(arr[0].getType(), Value::MOD8);

	arr.setWriteable(false);
	BOOST_CHECK_EQUAL(arr.isWriteable(), false);
}




BOOST_AUTO_TEST_CASE(ArrayTest_Constructor_1)
{
	Array arr(100);
	BOOST_CHECK_EQUAL(arr.size(), 100);
}




BOOST_AUTO_TEST_CASE(ArrayTest_Constructor_2)
{
	Array arr(100, Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(arr.size(), 100);
}




/**
Тест функции копирования массива
*/
BOOST_AUTO_TEST_CASE(ArrayTest_create)
{
	Array arr(3, Value::MOD8);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;

	Array arr_ = arr;
	arr_[0] = 0;
	BOOST_CHECK_EQUAL(arr[0].getValue(), 0);
	arr_.resize(5);
	BOOST_CHECK_EQUAL(arr.size(), 5);

	arr = Array(3, Value::MOD8);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;

	arr_ = arr.createNoLink();
	arr_[0] = 0;
	BOOST_CHECK_EQUAL(arr[0].getValue(), 1);
	arr_.resize(5);
	BOOST_CHECK_EQUAL(arr.size(), 3);
}
BOOST_AUTO_TEST_SUITE_END();
