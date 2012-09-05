#include "CodeExecuter.h"


CodeExecuter::CodeExecuter()
{
}



CodeExecuter::~CodeExecuter()
{
}




void CodeExecuter::exec()
{
	init();

	for(;m_contexts.top().atEnd();)
	{
		switch(exec_command())
		{
			case Exception::NumericError:
			{
				break;
			}

			case Exception::ConstraintError:
			{
				break;
			}
			case Exception::NoType:
			{
				break;
			}
		};
	}
}



void CodeExecuter::init()
{
	if(Program::getInstance().functionIsExists("main") == false)
		throw std::runtime_error("function with name main not exists");

	Context cntx;
	cntx.setFunction(Program::getInstance().getFunction("main").copy());
	m_contexts.push(cntx);
}




Exception::Type CodeExecuter::exec_command()
{
	return Exception::NoType;
}

