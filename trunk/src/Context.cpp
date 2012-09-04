#include "Context.h"

Context::Context()
{
	m_ip = 0;
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
	m_ip = 0;
}
