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




bool Function::exceptionHandlerIsExists(Exception::Type except) const
{
	return m_handlers.find(except) != m_handlers.end();
}




std::vector<Command> Function::getExceptionHandlerCode(Exception::Type except) const
{
	if(exceptionHandlerIsExists(except))
	{
		std::map<Exception::Type, std::vector<Command> >::const_iterator itr = m_handlers.find(except);
		return itr -> second;
	}
	else
		throw std::runtime_error(std::string("Handler for exception with name ") + 
			Exception::exceptionTypeToStr(except) + " not exists");
}




Function Function::copy()
{
	Function func;

	func.setName(m_name);
	DataKeeper keeper;

	std::list<std::string> names = getDataKeeperPtr() -> getValuesNames();
	std::list<std::string>::const_iterator itrNames = names.begin();

	for(;itrNames != names.end(); itrNames++)
		keeper.addVar(getDataKeeperPtr() -> getVarValue(*itrNames).createNoLink(), *itrNames);

	names = getDataKeeperPtr() -> getArraysNames();

	for(itrNames = names.begin(); itrNames != names.end(); itrNames++)
		keeper.addArray(getDataKeeperPtr() -> getArray(*itrNames).createNoLink(), *itrNames);

	func.setDataKeeper(keeper);

	func.m_args = m_args;
	func.m_argsIsRefs = m_argsIsRefs;

	func.m_code = codeCopy(func.getDataKeeperPtr(), m_code);
 	
	if(exceptionHandlerIsExists(Exception::NumericError))
		func.m_handlers[Exception::NumericError] = 
				codeCopy(func.getDataKeeperPtr(), getExceptionHandlerCode(Exception::NumericError));

	if(exceptionHandlerIsExists(Exception::ConstraintError))
		func.m_handlers[Exception::ConstraintError] = 
				codeCopy(func.getDataKeeperPtr(), getExceptionHandlerCode(Exception::ConstraintError));

	return func;
}



std::vector<Command> Function::codeCopy(DataKeeper *pnewKeeper, const std::vector<Command> &code)
{
	std::vector<Command> newCode = code;

	boost::shared_ptr<ArrayOperand> parr;
	boost::shared_ptr<VarOperand> pvar;
	boost::shared_ptr<CallOperand> pcop;


	for(std::size_t i=0, size=code.size(); i<size; i++)
	{
		for(std::size_t j=0, numbOfElements=code[i].getNumberOfOperands(); j<numbOfElements; j++)
		{
			if((boost::dynamic_pointer_cast<ArrayOperand, Operand>(code[i].getOperand(j))).get() != NULL)
			{
				parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(code[i].getOperand(j));
				ArrayOperand arr = parr -> convert(getDataKeeperPtr(), pnewKeeper);
				*(parr.get()) = arr;
			}
			else if((boost::dynamic_pointer_cast<VarOperand, Operand>(code[i].getOperand(j))).get() != NULL)
			{
				pvar = boost::dynamic_pointer_cast<VarOperand, Operand>(code[i].getOperand(j));
				VarOperand var = pvar -> convert(getDataKeeperPtr(), pnewKeeper);
				*(pvar.get()) = var;
			}
			else if((boost::dynamic_pointer_cast<CallOperand, Operand>(code[i].getOperand(j))).get() != NULL)
			{
				pcop = boost::dynamic_pointer_cast<CallOperand, Operand>(code[i].getOperand(j));
				CallOperand cop = pcop -> convert(getDataKeeperPtr(), pnewKeeper);
				*(pcop.get()) = cop;
			}

		}

	}

	return newCode;
}
