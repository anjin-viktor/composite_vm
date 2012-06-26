#include "CodeTranslator.h"


CodeTranslator::CodeTranslator()
{
	m_grammar.setOperationPtr(&m_op);
	m_grammar.setLabelNamesListPtr(&m_lbls);
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



void CodeTranslator::translate(std::string str)
{
	m_grammar.clear();
    std::string::iterator begin = str.begin(), end = str.end();

    bool success = qi::parse(begin, end, m_grammar);

    if(!success || begin != end)
    {
		m_lbls.clear();
		m_op = Command::NONE;
		throw ParseError("stopped at: " + std::string(begin, end));
	}
}




Command::Operation CodeTranslator::getOperation() const
{
	return m_op;
}



std::list<std::string> CodeTranslator::getLabelsList() const
{
	return m_lbls;
}