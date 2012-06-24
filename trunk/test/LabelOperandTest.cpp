#include <stdexcept>
#include <ctime>

#include <boost/test/unit_test.hpp>
#include "../src/LabelOperand.h"



/**
@file LabelOperandTest.cpp
@brief Набор модульных тестов для класса LabelOperand.
*/


BOOST_AUTO_TEST_SUITE(LABELOPERAND_TEST_SUITE);


/**
Тест функций LabelOperand::setLabelName, Label::getLabelName.
*/

BOOST_AUTO_TEST_CASE(testLabelOperand_SetGetName)
{
	LabelOperand lbl("lbl");
	BOOST_CHECK_EQUAL(lbl.getLabelName(), "lbl");

	lbl.setLabelName("new_name");
	BOOST_CHECK_EQUAL(lbl.getLabelName(), "new_name");

	BOOST_CHECK_THROW(lbl.setLabelName(".test"), std::logic_error);
	BOOST_CHECK_THROW(lbl.setLabelName("1lbl"), std::logic_error);

}


/**
Тест функции LabelOperand::checkLabelName().
*/

BOOST_AUTO_TEST_CASE(testLabelOperand_CheckLabelName)
{
	BOOST_CHECK_EQUAL(LabelOperand::checkLabelName("label"), true);
	BOOST_CHECK_EQUAL(LabelOperand::checkLabelName("l1230"), true);
	BOOST_CHECK_EQUAL(LabelOperand::checkLabelName("_1"), true);
	BOOST_CHECK_EQUAL(LabelOperand::checkLabelName("1label"), false);
	BOOST_CHECK_EQUAL(LabelOperand::checkLabelName("L1"), true);
}



BOOST_AUTO_TEST_SUITE_END();
