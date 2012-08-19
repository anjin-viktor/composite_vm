#include "Function.h"


Function::Function()
{
}


Function::~Function()
{
}


void Function::setName(const std::string &name)
{
	m_name = name;
}


std::string Function::getName() const
{
	return m_name;
}


void Function::setExceptionHandler(Exception::Type type, const std::vector<Command> &commands)
{
	m_handlers[type] = commands;
}


void Function::setCommands(const std::vector<Command> &commands)
{
	m_code = commands;
}
