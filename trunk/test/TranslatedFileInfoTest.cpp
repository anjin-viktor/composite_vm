#include <stdexcept>

#include <boost/test/unit_test.hpp>
#include "../src/TranslatedFileInfo.h"


#include <iostream>

/**
@file TranslatedFileInfoTest.cpp
@brief Набор модульных тестов для класса TranslatedFileInfo.
*/




BOOST_AUTO_TEST_SUITE(TRANSLATEDFILEINFO_TEST_SUITE);


/**
Тест класса
*/

BOOST_AUTO_TEST_CASE(TRANSLATED_FILE_INFO_Test)
{
	TranslatedFileInfo::getInstance().setFile("file");
	TranslatedFileInfo::getInstance().clearLineNo();

	BOOST_CHECK_EQUAL(TranslatedFileInfo::getInstance().getFile(), "file");
	BOOST_CHECK_EQUAL(TranslatedFileInfo::getInstance().getLineNo(), 0);

	TranslatedFileInfo::getInstance().incLineNo();
	TranslatedFileInfo::getInstance().incLineNo();
	TranslatedFileInfo::getInstance().incLineNo();
	TranslatedFileInfo::getInstance().incLineNo();
	TranslatedFileInfo::getInstance().incLineNo();

	BOOST_CHECK_EQUAL(TranslatedFileInfo::getInstance().getLineNo(), 5);

	TranslatedFileInfo::getInstance().setFile("file__");
	TranslatedFileInfo::getInstance().clearLineNo();

	BOOST_CHECK_EQUAL(TranslatedFileInfo::getInstance().getFile(), "file__");
	BOOST_CHECK_EQUAL(TranslatedFileInfo::getInstance().getLineNo(), 0);
}



BOOST_AUTO_TEST_SUITE_END();
