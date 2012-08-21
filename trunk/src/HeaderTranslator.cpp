#include "HeaderTranslator.h"


HeaderTranslator::HeaderTranslator()
{
	m_pdata = NULL;
}



HeaderTranslator::~HeaderTranslator()
{
}




void HeaderTranslator::setDataKeeperPtr(DataKeeper *pdata)
{
	m_pdata = pdata;
	m_grammar.setDataKeeperPtr(pdata);
}




std::string HeaderTranslator::getName() const
{
	return m_grammar.getName();
}




void HeaderTranslator::translate(std::string str)
{
    std::string::iterator begin = str.begin(), end = str.end();

    m_grammar.clear();

    if(m_pdata)
    	m_pdata -> clear();

    bool success = qi::parse(begin, end, m_grammar);

    if(!success || begin != end)
		throw ParseError("stopped at: " + std::string(begin, end));
}
