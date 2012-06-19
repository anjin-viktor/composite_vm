#include "VarTranslator.h"


VarTranslator::VarTranslator()
{
	m_grammar.setNamePtr(&m_valName);
	m_grammar.setValuePtr(&m_val);
}




std::pair<std::string, Value> VarTranslator::translate(std::string str)
{
	m_grammar.clearValue();
    std::string::iterator begin = str.begin(), end = str.end();

    bool success = qi::parse(begin, end, m_grammar);

    if(!success || begin != end)
		throw ParseError("stopped at: " + std::string(begin, end));

	return std::pair<std::string, Value>(m_valName, m_val);
}
