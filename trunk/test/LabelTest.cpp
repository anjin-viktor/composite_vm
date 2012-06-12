#define BOOST_TEST_MODULE COMPOZIT_VM_LABEL

#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/Label.h"



/**
@file LabelTest.cpp
@brief Набор модульных тестов для класса Label
*/

/**
Тест функций Label::setLabelName, Label::getLabelName
*/

BOOST_AUTO_TEST_CASE(testLabel_SetGetName)
{
	Label lbl("lbl");
	BOOST_CHECK_EQUAL(lbl.getLabelName(), "lbl");

	lbl.setLabelName("new_name");
	BOOST_CHECK_EQUAL(lbl.getLabelName(), "new_name");
}


/**
Тест функции Label::checkLabelName()
*/

BOOST_AUTO_TEST_CASE(testLabel_CheckLabelName)
{
	BOOST_CHECK_EQUAL(Label::checkLabelName("label"), true);
	BOOST_CHECK_EQUAL(Label::checkLabelName("l1230"), true);
	BOOST_CHECK_EQUAL(Label::checkLabelName("_1"), true);
	BOOST_CHECK_EQUAL(Label::checkLabelName("1label"), false);
	BOOST_CHECK_EQUAL(Label::checkLabelName("L1"), true);
}