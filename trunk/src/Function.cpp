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



void Function::clear()
{
	m_name = "";
	m_handlers.clear();
	m_code.clear();
}



void Function::setDataKeeper(const DataKeeper &keeper)
{
	m_data = keeper;
}



DataKeeper &Function::getDataKeeper()
{
	return m_data;
}



const DataKeeper &Function::getDataKeeper() const
{
	return m_data;
}



DataKeeper *Function::getDataKeeperPtr()
{
	return &m_data;
}



std::vector<Command> Function::getCommands() const
{
	return m_code;
}




std::list<std::string> Function::getArgsNames() const
{
	return m_args;
}



void Function::addArgName(const std::string &name) 
{
	m_args.push_back(name);
}




void Function::setArgsNamesFromList(const std::list<std::string> &lst)
{
	m_args = lst;
}