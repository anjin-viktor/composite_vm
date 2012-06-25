#include "CodeTranslator.h"


CodeTranslator::CodeTranslator()
{
}




CodeTranslator::~CodeTranslator()
{
}




void CodeTranslator::setDataKeeper(const DataKeeper &keeper)
{
	m_data = keeper;
}




DataKeeper CodeTranslator::getDataKeeper() const
{
	return m_data;
}



void CodeTranslator::translate(const std::string &str)
{
}

