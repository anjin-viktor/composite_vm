#define BOOST_TEST_MODULE COMPOZIT_VM_VALUE

#include <stdexcept>
#include <ctime>
#include <cstdlib>

#include <boost/test/unit_test.hpp>
#include "../src/Value.h"


/**
@file ValueTest.cpp
@brief Набор модульных тестов для класса Value.
*/


BOOST_AUTO_TEST_SUITE(VALUE_TEST_SUITE);

/**
Тест конструктора по умолчанию.
*/


BOOST_AUTO_TEST_CASE(testValueDefaultConstructor)
{
	Value val;
	BOOST_CHECK_EQUAL(val.isReadable(), false);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);
	BOOST_CHECK_EQUAL(val.getType(), Value::NO_TYPE);
	BOOST_CHECK_THROW(val.getValue(), std::runtime_error);
}

/**
Тест конструктора Value::Value(long long val).
*/

BOOST_AUTO_TEST_CASE(tastValueConstructor1)
{
	Value val(129);

	BOOST_CHECK_EQUAL(val.isReadable(), false);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(val.getValue(), 129);

	val = Value(-125);

	BOOST_CHECK_EQUAL(val.isReadable(), false);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);
	BOOST_CHECK_EQUAL(val.getType(), Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(val.getValue(), -125);


	val = Value(1000);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_SHORT);
	BOOST_CHECK_EQUAL(val.getValue(), 1000);


	val = Value(-129);
	BOOST_CHECK_EQUAL(val.getType(), Value::SIGNED_SHORT);
	BOOST_CHECK_EQUAL(val.getValue(), -129);

	val = Value(1 << 16);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_INT);
	BOOST_CHECK_EQUAL(val.getValue(), 1 << 16);


	val = Value(-(1 << 15));
	BOOST_CHECK_EQUAL(val.getType(), Value::SIGNED_SHORT);
	BOOST_CHECK_EQUAL(val.getValue(), -(1 << 15));


	val = Value(-(1 << 15) - 1);
	BOOST_CHECK_EQUAL(val.getType(), Value::SIGNED_INT);
	BOOST_CHECK_EQUAL(val.getValue(), -(1 << 15) - 1);

	BOOST_CHECK_THROW(Value((long long)1 << 32), std::runtime_error);
	BOOST_CHECK_THROW(Value(-((long long)1 << 32)), std::runtime_error);
}



/**
Тест конструктора Value::Value(long long val, bool readable, bool writeable).
*/

BOOST_AUTO_TEST_CASE(testValueConstructor2)
{
	Value val(129, true, true);
	BOOST_CHECK_EQUAL(val.isReadable(), true);
	BOOST_CHECK_EQUAL(val.isReadable(), true);
}


/**
Тест конструктора Value::Value(long long val, Value::ValueType type, bool readable, bool writeable).
*/

BOOST_AUTO_TEST_CASE(testValueConstructor3)
{
	Value val(129, Value::MOD8, true, false);
	BOOST_CHECK_EQUAL(val.isReadable(), true);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);
	BOOST_CHECK_EQUAL(val.getValue(), (unsigned char) 129);

	Value val_((long long) 1 << 35, Value::SIGNED_INT, false, true);
	BOOST_CHECK_EQUAL(val_.isReadable(), false);
	BOOST_CHECK_EQUAL(val_.isWriteable(), true);
	BOOST_CHECK_EQUAL(val_.getValue(), (signed int) ((long long) 1 << 35));
}


/**
Тест функций set/getReadable/Writeable.
*/

BOOST_AUTO_TEST_CASE(testValuePermissions)
{
	Value val;
	val.setReadable(true);
	BOOST_CHECK_EQUAL(val.isReadable(), true);

	val.setReadable(false);
	BOOST_CHECK_EQUAL(val.isReadable(), false);


	val.setWriteable(false);
	BOOST_CHECK_EQUAL(val.isWriteable(), false);

	val.setWriteable(true);
	BOOST_CHECK_EQUAL(val.isWriteable(), true);
}



/**
Тест функций set/getValue.
*/

BOOST_AUTO_TEST_CASE(testValue_SET_GET_Value)
{
	Value val(-10000);
	BOOST_CHECK_EQUAL(val.getValue(), -10000);

	val.setType(Value::SIGNED_SHORT);
	BOOST_CHECK_EQUAL(val.getValue(), -10000);

	val.setType(Value::UNSIGNED_SHORT);
	BOOST_CHECK_EQUAL(val.getValue(), 55536);

	val.setType(Value::SIGNED_CHAR);
	BOOST_CHECK_EQUAL(val.getValue(), (signed char) -16);

	val.setType(Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(val.getValue(), (unsigned char) 240);

	val.setType(Value::MOD8);
	BOOST_CHECK_EQUAL(val.getValue(), (unsigned char) 240);
}



/**
Тест функций set/getType.
*/


BOOST_AUTO_TEST_CASE(testValue_SET_GET_TYPE)
{
	Value val;
	val.setType(Value::UNSIGNED_CHAR);
	BOOST_CHECK_EQUAL(val.getType(), Value::UNSIGNED_CHAR);

	Value val1(1, Value::SIGNED_INT, true, true);
	BOOST_CHECK_EQUAL(val1.getType(), Value::SIGNED_INT);
	val1.setType(Value::SIGNED_SHORT);
	BOOST_CHECK_EQUAL(val1.getType(), Value::SIGNED_SHORT);
	BOOST_CHECK_THROW(val.getValue(), std::runtime_error);


}



/**
Тест взятия значения по ссылке и не по ссылке.
*/


BOOST_AUTO_TEST_CASE(testValue_LINK)
{
	Value val(5);
	Value n_val = val;
	n_val.setValue(1);

	BOOST_CHECK_EQUAL(val.getValue(), 1);

	Value a;
	a = n_val;
	a.setValue(2);

	BOOST_CHECK_EQUAL(val.getValue(), 2);


	Value c = val.createNoLink();
	c.setValue(3);
	BOOST_CHECK_EQUAL(val.getValue(), 2);
}





/**
Тест на функцию isOwerflow
*/


BOOST_AUTO_TEST_CASE(testValue_ISOWERFLOF)
{

	BOOST_CHECK_EQUAL(Value::isOverflow(256, Value::SIGNED_CHAR), true);
	BOOST_CHECK_EQUAL(Value::isOverflow(256, Value::UNSIGNED_CHAR), true);
	BOOST_CHECK_EQUAL(Value::isOverflow(256, Value::MOD8), false);

	BOOST_CHECK_EQUAL(Value::isOverflow(-10, Value::SIGNED_CHAR), false);
	BOOST_CHECK_EQUAL(Value::isOverflow(-10, Value::UNSIGNED_CHAR), true);
	BOOST_CHECK_EQUAL(Value::isOverflow(-10, Value::MOD8), false);

	BOOST_CHECK_EQUAL(Value::isOverflow(1000, Value::SIGNED_INT), false);
	BOOST_CHECK_EQUAL(Value::isOverflow(1000, Value::UNSIGNED_INT), false);
	BOOST_CHECK_EQUAL(Value::isOverflow(1000, Value::MOD16), false);
}




BOOST_AUTO_TEST_SUITE_END();
