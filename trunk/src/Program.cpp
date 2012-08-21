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
	{
		/*Бросить исключение*/
	}

	return m_functions[name];
}



/*
const Function &Program::getFunction(const std::string &name) const
{
	if(functionIsExists(name) == false)
	{
		/*Бросить исключение*/
/*	}

	return	m_functions[name];
}

*/


void Program::addFunction(const Function &function)
{
	if(functionIsExists(function.getName()) != false)
	{
		/*Бросить исключение*/
	}

	m_functions[function.getName()] = function;
}




bool Program::functionIsExists(const std::string &name) const
{
	return m_functions.find(name) != m_functions.end();	
}
