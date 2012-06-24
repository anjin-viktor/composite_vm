#include "DataKeeper.h"



DataKeeper::DataKeeper()
{
}




DataKeeper::~DataKeeper()
{
}




void DataKeeper::addVar(const Value &val, const std::string &name) throw(std::runtime_error)
{
	if(isExists(name) == false)
		m_vars[name] = val;
	else
		throw(std::runtime_error(std::string("Variable or array with name ") + name + " alredy exists"));
}




void DataKeeper::addArray(const Array &arr, const std::string &name) throw(std::runtime_error)
{
	if(isExists(name) == false)
		m_arrs[name] = arr;
	else
		throw(std::runtime_error(std::string("Variable or array with name ") + name + " alredy exists"));
}





bool DataKeeper::isExists(const std::string &name) const
{
	return (m_vars.find(name) != m_vars.end() || m_arrs.find(name) != m_arrs.end());
}




bool DataKeeper::isVar(const std::string &name) const
{
	return (m_vars.find(name) != m_vars.end());
}




bool DataKeeper::isArray(const std::string &name) const
{
	return (m_arrs.find(name) != m_arrs.end());
}




Value &DataKeeper::getVarValue(const std::string &name) throw(std::runtime_error)
{
	std::map<std::string, Value>::iterator itr = m_vars.find(name);
	if(itr == m_vars.end())
		throw(std::runtime_error("Variable with name + " + name + " not exists"));

	return itr -> second;
}




const Value &DataKeeper::getVarValue(const std::string &name) const throw(std::runtime_error)
{
	std::map<std::string, Value>::const_iterator itr = m_vars.find(name);
	if(itr == m_vars.end())
		throw(std::runtime_error("Variable with name + " + name + " not exists"));

	return itr -> second;
}





Array &DataKeeper::getArray(const std::string &name) throw(std::runtime_error)
{
	std::map<std::string, Array>::iterator itr = m_arrs.find(name);

	if(itr == m_arrs.end())
		throw(std::runtime_error("Array with name " + name + " not exists"));

	return itr -> second;
}





const Array &DataKeeper::getArray(const std::string &name) const throw(std::runtime_error)
{
	std::map<std::string, Array>::const_iterator itr = m_arrs.find(name);

	if(itr == m_arrs.end())
		throw(std::runtime_error("Array with name + " + name + " not exists"));

	return itr -> second;
}
