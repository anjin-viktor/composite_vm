#include "DataKeeper.h"



DataKeeper::DataKeeper()
{
}




DataKeeper::~DataKeeper()
{
}




void DataKeeper::addVar(const Value &val, const std::string &name) throw(ParseError)
{
	if(m_vars.find(name) == m_vars.end() && m_arrs.find(name) == m_arrs.end())
		m_vars[name] = val;
	else
		throw(ParseError(std::string("Variable or array with name ") + name + " alredy exists"));
}




void DataKeeper::addArray(const Array &arr, const std::string &name) throw(ParseError)
{
	if(m_vars.find(name) == m_vars.end() && m_arrs.find(name) == m_arrs.end())
		m_arrs[name] = arr;
	else
		throw(ParseError(std::string("Variable or array with name ") + name + " alredy exists"));
}
