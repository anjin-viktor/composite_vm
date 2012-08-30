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
	m_argsIsRefs[name] = false;
}




void Function::setArgsNamesFromList(const std::list<std::string> &lst)
{
	m_args = lst;
	m_argsIsRefs.clear();

	std::list<std::string>::const_iterator itr = lst.begin();
	for(;itr != lst.end(); itr++)
		m_argsIsRefs[*itr] = false;
}



void  Function::argIsRef(const std::string &name, bool f)
{
	m_argsIsRefs[name] = f;
}


bool Function::argIsRef(const std::string &name) const
{
	std::map<std::string, bool>::const_iterator itr = m_argsIsRefs.find(name);

	if(itr == m_argsIsRefs.end())
		throw std::runtime_error(std::string("Argument with name ") + name + " not exists");

	return itr -> second;
}
