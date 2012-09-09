#include "Context.h"

Context::Context()
{
	m_ip = 0;
	m_currentCode = Exception::NoType;
	m_cmpRes = NotDefined;
}


Context::Context(const Context &cntx)
{
	m_function = cntx.m_function;
	m_cmpRes = cntx.m_cmpRes;

	m_currentCode = cntx.m_currentCode;
	m_ip = cntx.m_ip;

	if(m_currentCode == Exception::NoType)
			m_code = m_function.getCommands();
	else
		m_code = m_function.getExceptionHandlerCode(m_currentCode);
}

Context::~Context()
{
}


void Context::setFunction(const Function &f)
{
	m_function = f;
}



void Context::init()
{
	m_ip = 0;
	m_code = m_function.getCommands();
	m_currentCode = Exception::NoType;
	m_cmpRes = NotDefined;
}




bool Context::atEnd() const
{
	return m_code.size() <= m_ip;
}




bool Context::handlerIsExists(Exception::Type exceptType) const
{
	return m_function.exceptionHandlerIsExists(exceptType);
}



void Context::execHandler(Exception::Type exceptType)
{
	m_code = m_function.getExceptionHandlerCode(exceptType);
	m_currentCode = exceptType;
	m_ip = 0;
}




Function *Context::getFunctionPtr() 
{
	return &m_function;
}




Context &Context::operator =(const Context &cntx)
{
	if(this == &cntx)
		return  *this;
	
	m_function = cntx.m_function;

	m_currentCode = cntx.m_currentCode;
	m_ip = cntx.m_ip;

	if(m_currentCode == Exception::NoType)
			m_code = m_function.getCommands();
	else
		m_code = m_function.getExceptionHandlerCode(m_currentCode);

	m_cmpRes = cntx.m_cmpRes;

	return *this;
}




Exception::Type Context::currentCode() const
{
	return m_currentCode;
}