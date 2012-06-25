#include <stdexcept>


#include <boost/test/unit_test.hpp>
#include "../src/Label.h"



/**
@file LabelTest.cpp
@brief Набор модульных тестов для класса Label.
*/


BOOST_AUTO_TEST_SUITE(LABEL_TEST_SUITE);


/**
Тест конструкторов, функций установки и получения имени метки и её адреса.
*/

BOOST_AUTO_TEST_CASE(testLabel_SetGetName)
{
	Label lbl("lbl", 1);
	BOOST_CHECK_EQUAL(lbl.getLabelName(), "lbl");
	BOOST_CHECK_EQUAL(lbl.getLabelAddr(), 1);


	lbl.setLabelName("new_name");
	lbl.setLabelAddr(2);
	BOOST_CHECK_EQUAL(lbl.getLabelName(), "new_name");
	BOOST_CHECK_EQUAL(lbl.getLabelAddr(), 2);


	BOOST_CHECK_THROW(lbl.setLabelName(".test"), std::logic_error);
	BOOST_CHECK_THROW(lbl.setLabelName("1lbl"), std::logic_error);
}





BOOST_AUTO_TEST_SUITE_END();
