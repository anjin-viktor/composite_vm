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

			case Exception::ProgramError:
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
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res = pfArg -> getValue();
			res += static_cast<long long>(psArg -> getValue());
			
			if(Value::isOverflow(res, pfArg -> getAfterCastType()))
				return Exception::NumericError;


			pfArg -> setValue(res);

			m_contexts.top().m_ip++;

			break;
		}
		case Command::SUB:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res = pfArg -> getValue();
			res -= static_cast<long long>(psArg -> getValue());
			
			if(Value::isOverflow(res, pfArg -> getAfterCastType()))
				return Exception::NumericError;


			pfArg -> setValue(res);

			m_contexts.top().m_ip++;

			break;
		}
		case Command::MUL:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res = pfArg -> getValue();
			res *= static_cast<long long>(psArg -> getValue());
			

			if(Value::isOverflow(res, pfArg -> getAfterCastType()))
				return Exception::NumericError;

			if((res / static_cast<long long>(psArg -> getValue())) != pfArg -> getValue())
				return Exception::NumericError;

			pfArg -> setValue(res);
			m_contexts.top().m_ip++;

			break;
		}
		case Command::DIV:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res = pfArg -> getValue();
			res /= static_cast<long long>(psArg -> getValue());

			pfArg -> setValue(res);
			m_contexts.top().m_ip++;

			break;
		}
		case Command::MOD:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res = pfArg -> getValue();
			res %= static_cast<long long>(psArg -> getValue());

			pfArg -> setValue(res);
			m_contexts.top().m_ip++;

			break;
		}
		case Command::JMP:
		{
			boost::shared_ptr<LabelOperand> plblArg = boost::dynamic_pointer_cast<LabelOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());

			m_contexts.top().m_ip = plblArg -> getCommandOffset();

			break;
		}
		case Command::JL:
		{
			if(m_contexts.top().m_cmpRes & Context::NotDefined)
				return Exception::ProgramError;

			boost::shared_ptr<LabelOperand> plblArg = boost::dynamic_pointer_cast<LabelOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());


			if(!(m_contexts.top().m_cmpRes & Context::Equal) && !(m_contexts.top().m_cmpRes & Context::Greater))
				m_contexts.top().m_ip = plblArg -> getCommandOffset();
			else
				m_contexts.top().m_ip++;

			break;
		}
		case Command::JE:
		{
			if(m_contexts.top().m_cmpRes & Context::NotDefined)
				return Exception::ProgramError;

			boost::shared_ptr<LabelOperand> plblArg = boost::dynamic_pointer_cast<LabelOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());

			if(m_contexts.top().m_cmpRes & Context::Equal)
				m_contexts.top().m_ip = plblArg -> getCommandOffset();
			else
				m_contexts.top().m_ip++;

			break;
		}
		case Command::JG:
		{
			if(m_contexts.top().m_cmpRes & Context::NotDefined)
				return Exception::ProgramError;

			boost::shared_ptr<LabelOperand> plblArg = boost::dynamic_pointer_cast<LabelOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());

			if(m_contexts.top().m_cmpRes & Context::Greater)
				m_contexts.top().m_ip = plblArg -> getCommandOffset();
			else
				m_contexts.top().m_ip++;

			break;
		}
		case Command::JNE:
		{
			if(m_contexts.top().m_cmpRes & Context::NotDefined)
				return Exception::ProgramError;

			boost::shared_ptr<LabelOperand> plblArg = boost::dynamic_pointer_cast<LabelOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());

			if(!(m_contexts.top().m_cmpRes & Context::Equal))
				m_contexts.top().m_ip = plblArg -> getCommandOffset();
			else
				m_contexts.top().m_ip++;

			break;
		}
		case Command::AOUT:
		{
			Array *parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand()) -> getArrayPtr();

			char *printingStr = new char[parr -> size() + 2];
			printingStr[parr -> size()] = '\n';
			printingStr[parr -> size()+1] = '\0';

			for(std::size_t i=0, size=parr->size(); i<size; i++)
			{
				if(parr -> operator[](i).isReadable() == false)
					return Exception::ConstraintError;

				printingStr[i] = static_cast<char>((parr -> operator[](i)).getValue());
			}

			m_poutput -> write(printingStr, parr -> size()+1);
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
			m_contexts.top().m_ip++;
			break;
		}
		case Command::CMP:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			m_contexts.top().m_cmpRes &= 0;

			long long res = pfArg -> getValue();
			res -= static_cast<long long>(psArg -> getValue());

			if(res == 0)
				m_contexts.top().m_cmpRes |= Context::Equal;
			else if(res > 0)
				m_contexts.top().m_cmpRes |= Context::Greater;

			m_contexts.top().m_ip++;
			break;
		}
		case Command::NONE:
		{
			break;
		}
	};

	return Exception::NoType;
}
