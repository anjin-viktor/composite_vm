#include "CodeExecuter.h"




CodeExecuter::CodeExecuter(): m_poutput(&std::cout)
{
}



CodeExecuter::~CodeExecuter()
{
}




void CodeExecuter::exec()
{
	init();


	for(;m_contexts.top().atEnd() == false;)
	{
		switch(exec_command())
		{
			case Exception::NumericError:
			{
				std::cerr << "NumericError\n";
				exit(1);
				break;
			}

			case Exception::ConstraintError:
			{
				std::cerr << "ConstraintError\n";
				exit(1);
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
	cntx.init();
	m_contexts.push(cntx);

}




void CodeExecuter::setOutputStream(std::ostream &stream)
{
	m_poutput = &stream;
}



Exception::Type CodeExecuter::exec_command()
{
	switch(m_contexts.top().m_code[m_contexts.top().m_ip].getOperationType())
	{
		case Command::MOV:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			

			if(pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			pfArg -> setValue(psArg -> getValue());
			m_contexts.top().m_ip++;

			break;
		}
		case Command::ADD:
		{

			break;
		}
		case Command::SUB:
		{

			break;
		}
		case Command::MUL:
		{

			break;
		}
		case Command::DIV:
		{

			break;
		}
		case Command::MOD:
		{

			break;
		}
		case Command::JMP:
		{

			break;
		}
		case Command::JL:
		{

			break;
		}
		case Command::JE:
		{

			break;
		}
		case Command::JG:
		{

			break;
		}
		case Command::JNE:
		{

			break;
		}
		case Command::AOUT:
		{
			Array *parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand()) -> getArrayPtr();

			char *printingStr = new char[parr -> size() + 1];
			printingStr[parr -> size()] = '\0';

			for(std::size_t i=0, size=parr->size(); i<size; i++)
			{
				if(parr -> operator[](i).isReadable() == false)
					return Exception::ConstraintError;

				printingStr[i] = static_cast<char>((parr -> operator[](i)).getValue());
			}

			m_poutput -> write(printingStr, parr -> size());
			delete [] printingStr;
			m_contexts.top().m_ip++;

			break;
		}
		case Command::RSZ:
		{

			break;
		}
		case Command::CALL:
		{

			break;
		}
		case Command::RET:
		{

			break;
		}
		case Command::NOP:
		{

			break;
		}
		case Command::CMP:
		{

			break;
		}
		case Command::NONE:
		{

			break;
		}
	};

	return Exception::NoType;
}
