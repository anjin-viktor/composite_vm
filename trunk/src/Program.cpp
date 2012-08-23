#include "Program.h"

Program Program::m_instance;


Program::Program()
{
}



Program::~Program()
{
}



Program &Program::getInstance()
{
	return m_instance;
}



Function &Program::getFunction(const std::string &name)
{
	if(functionIsExists(name) == false)
		throw std::runtime_error("function with name " + name + " not exists");

	return m_functions[name];
}




const Function &Program::getFunction(const std::string &name) const
{
	if(functionIsExists(name) == false)
		throw std::runtime_error("function with name " + name + " not exists");

	std::map<std::string, Function>::const_iterator itr = m_functions.find(name);
	return itr -> second;
}




void Program::addFunction(const Function &function)
{
	if(functionIsExists(function.getName()) != false)
		throw std::runtime_error("function with name " + function.getName() + " already exists");

	m_functions[function.getName()] = function;
}




bool Program::functionIsExists(const std::string &name) const
{
	return m_functions.find(name) != m_functions.end();	
}




std::size_t Program::numberOfFunctions() const
{
	return m_functions.size();
}




std::list<std::string> Program::getFunctionNames() const
{
	std::list<std::string> lst;
	std::map<std::string, Function>::const_iterator itr = m_functions.begin();

	for(;itr != m_functions.end(); itr++)
		lst.push_front(itr -> first);

	return lst;
}



void Program::clear()
{
	m_functions.clear();
}