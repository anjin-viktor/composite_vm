#include "VarTranslator.h"


VarTranslator::VarTranslator()
{
	m_grammar.setNamePtr(&m_name);
	m_grammar.setValuePtr(&m_val);
	m_grammar.setArrayPtr(&m_arr);
}




void VarTranslator::translate(std::string str)
{
	m_grammar.clear();
    std::string::iterator begin = str.begin(), end = str.end();

    bool success = qi::parse(begin, end, m_grammar);

    if(!success || begin != end)
		throw ParseError("stopped at: " + std::string(begin, end));

    std::string name = getName();

    if(m_data.isExists(name))
    	throw ParseError("Variable or array with name + " + name + " already exists");

   	if(isArray())
   		m_data.addArray(getArray(), name);
   	else if(isVariable())
   		m_data.addVar(getValue(), name);
}




bool VarTranslator::isArray() const
{
	return m_grammar.isArrayDefinition();
}



bool VarTranslator::isVariable() const
{
	return m_grammar.isVariableDefinition();
}




std::string VarTranslator::getName() const
{
	return m_name;
}




Value VarTranslator::getValue() const
{
	return m_val;
}




Array VarTranslator::getArray() const
{
	return m_arr;
}




void VarTranslator::clear()
{
	m_data.clear();
}




DataKeeper VarTranslator::getDataKeeper() const
{
	return m_data;
}
