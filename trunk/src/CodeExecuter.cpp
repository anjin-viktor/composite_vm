#include "CodeExecuter.h"




CodeExecuter::CodeExecuter(): m_poutput(&std::cout), m_perrStream(&std::cout)
{
}



CodeExecuter::~CodeExecuter()
{
}




void CodeExecuter::exec()
{
	init();


	for(;m_contexts.empty() == false;)
	{
		if(m_contexts.top().atEnd() == false)
		{
			Exception::Type except = exec_command();

			if(except != Exception::NoType)
			{
				if(m_contexts.top().currentCode() != Exception::NoType)
					m_contexts.pop();

				for(;m_contexts.empty() == false && m_contexts.top().handlerIsExists(except) == false; m_contexts.pop());


				if(m_contexts.empty() == false)
					m_contexts.top().execHandler(except);
				else
					defaultExceptionHandler(except);
			}
		}

		if(m_contexts.empty() == false && m_contexts.top().atEnd())
			m_contexts.pop();
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


			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;


			if(pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;



			try
			{
				psArg -> getValue();
				pfArg -> setValue(psArg -> getValue());

				pfArg -> initialize();
			}
			catch(std::runtime_error)
			{

				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;

			break;
		}
		case Command::ADD:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res;
			try
			{
				res = pfArg -> getValue();
				res += static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			
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

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;


			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;
			
			long long res;
			try
			{
				res = pfArg -> getValue();
				res -= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}


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

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res;
			try
			{
				res = pfArg -> getValue();
				res *= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			

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

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;


			long long res;
			try
			{
				res = pfArg -> getValue();
				res /= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}


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

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res;
			try
			{
				res = pfArg -> getValue();
				res %= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

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

			if(parr == NULL)
				return Exception::ConstraintError;

			char *printingStr = new char[parr -> size() + 2];
			printingStr[parr -> size()] = '\n';
			printingStr[parr -> size()+1] = '\0';

			for(std::size_t i=0, size=parr->size(); i<size; i++)
			{
				if(parr -> operator[](i).isReadable() == false)
					return Exception::ConstraintError;

				try
				{
					printingStr[i] = static_cast<char>((parr -> operator[](i)).getValue());
				}
				catch(std::runtime_error)
				{
					return Exception::ConstraintError;
				}
			}

			m_poutput -> write(printingStr, parr -> size()+1);
			delete [] printingStr;
			m_contexts.top().m_ip++;

			break;
		}
		case Command::RSZ:
		{
			Array *parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand()) -> getArrayPtr();

			boost::shared_ptr<VarOperand> pval = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(parr == NULL || pval -> hasValue() == false)
				return Exception::ConstraintError;

			if(parr -> isWriteable() == false || pval -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				try
				{
					parr -> resizeAndFillZeros(pval -> getValue());
				}
				catch(std::runtime_error)
				{
					return Exception::ConstraintError;
				}
			}
			catch(std::bad_alloc allocError)
			{
				return Exception::StorageError;
			}

			m_contexts.top().m_ip++;

			break;
		}
		case Command::CALL:
		{
			Command command = m_contexts.top().m_code[m_contexts.top().m_ip];
			std::string lblName = boost::dynamic_pointer_cast<LabelOperand, Operand>
				(command.getFirstOperand()) -> getLabelName();


			Function newFunc = Program::getInstance().getFunction(lblName).copy();



			std::list<std::string> argNames = newFunc.getArgsNames();
			std::list<std::string>::const_iterator itr = argNames.begin();

			for(std::size_t i=1, n=command.getNumberOfOperands(); i<n && itr != argNames.end(); i++, itr++)
			{
				boost::shared_ptr<CallOperand> pcallOp = boost::dynamic_pointer_cast<CallOperand, Operand>
					(command.getOperand(i));

				if(pcallOp -> isArray())
				{
					if(newFunc.argIsRef(*itr))
					{
						Array *pold = &(newFunc.getDataKeeperPtr() -> getArray(*itr));
						newFunc.getDataKeeperPtr() -> getArray(*itr) = pcallOp -> getArray();
						Array *pnew = &(newFunc.getDataKeeperPtr() -> getArray(*itr));
						newFunc.replace(pold, pnew);

					}
					else
					{
						Array *pold = &(newFunc.getDataKeeperPtr() -> getArray(*itr));
						newFunc.getDataKeeperPtr() -> getArray(*itr) = pcallOp -> getArray().createNoLink();
						Array *pnew = &(newFunc.getDataKeeperPtr() -> getArray(*itr));
						newFunc.replace(pold, pnew);
					}
				}
				else
				{
					if(newFunc.argIsRef(*itr))
					{
						Value *pold = &(newFunc.getDataKeeperPtr() -> getVarValue(*itr));
						newFunc.getDataKeeperPtr() -> getVarValue(*itr) = pcallOp -> getValue();
						Value *pnew = &(newFunc.getDataKeeperPtr() -> getVarValue(*itr));
						newFunc.replace(pold, pnew);
					}
					else
					{
						if(pcallOp -> getValue().isReadable() == false)
							return Exception::ConstraintError;
						try
						{
							newFunc.getDataKeeperPtr() -> getVarValue(*itr).setValue(pcallOp -> getValue().getValue());
						}
						catch(std::runtime_error)
						{
							return Exception::ConstraintError;
						}
					}
				}
			}

			Context cntx;
			cntx.setFunction(newFunc);

			cntx.init();

			m_contexts.top().m_ip++;
			m_contexts.push(cntx);

			break;
		}
		case Command::RET:
		{
			m_contexts.pop();
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

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

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
		case Command::GTEL:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> ptArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getOperand(2));
			boost::shared_ptr<ArrayOperand> psArg = boost::dynamic_pointer_cast<ArrayOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(psArg -> hasValue() == false || ptArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || ptArg -> isReadable() == false)
				return Exception::ConstraintError;


			try
			{
				long long  n = ptArg -> getValue();
				if(psArg -> getArrayPtr() -> size() <= n || n < 0)
					return Exception::ConstraintError;

				pfArg -> setValue((psArg -> getArrayPtr() -> operator[](n)).getValue());
				pfArg -> initialize();
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}
		case Command::STEL:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> ptArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getOperand(2));
			boost::shared_ptr<ArrayOperand> psArg = boost::dynamic_pointer_cast<ArrayOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false || ptArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || ptArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long  n = ptArg -> getValue();
				if(psArg -> getArrayPtr() -> size() <= n || n < 0)
					return Exception::ConstraintError;

				if((psArg -> getArrayPtr() -> operator[](n)).isWriteable() == false)
					return Exception::ConstraintError;

				(psArg -> getArrayPtr() -> operator[](n)).setValue(pfArg -> getValue());
				(psArg -> getArrayPtr() -> operator[](n)).setReadable(true);
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::SIZE:
		{
			Array *parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand()) -> getArrayPtr();

			boost::shared_ptr<VarOperand> pval = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());

			if(parr == NULL)
				return Exception::ConstraintError;

			if(pval -> isWriteable() == false)
				return Exception::ConstraintError;

			try
			{
				try
				{
					pval -> setValue(parr -> size());
				}
				catch(std::runtime_error)
				{
					return Exception::ConstraintError;
				}
			}
			catch(std::bad_alloc allocError)
			{
				return Exception::StorageError;
			}

			m_contexts.top().m_ip++;


			break;
		}

		case Command::SHL:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long tmp = pfArg -> getValue();
				long long after = tmp;
				long long n = psArg -> getValue();
				tmp <<= n;
				pfArg -> setValue(tmp);
				
				Value::ValueType type = pfArg -> getAfterCastType();

				if((pfArg -> getValue() >> n) != after &&
					(type != Value::MOD8 && type != Value::MOD16 && type != Value::MOD32)
				)
				{
					pfArg -> setValue(after);
					return Exception::NumericError;
				}

			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::SHR:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long tmp = pfArg -> getValue();
				long long after = tmp;
				long long n = psArg -> getValue();
				tmp >>= n;
				pfArg -> setValue(tmp);

				Value::ValueType type = pfArg -> getAfterCastType();
				if((pfArg -> getValue() << n) != after &&
					(type != Value::MOD8 && type != Value::MOD16 && type != Value::MOD32)
				)
				{
					pfArg -> setValue(after);
					return Exception::NumericError;
				}

			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::SHLWO:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long tmp = pfArg -> getValue();
				long long n = psArg -> getValue();
				tmp <<= n;
				pfArg -> setValue(tmp);
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::SHRWO:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long tmp = pfArg -> getValue();
				long long n = psArg -> getValue();
				tmp >>= n;
				pfArg -> setValue(tmp);
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::ROL:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long tmp = pfArg -> getValue();
				long long n = psArg -> getValue();
				unsigned long long mask = 1;
				int k;

				switch(pfArg -> getAfterCastType())
				{
					case Value::SIGNED_CHAR:
					case Value::UNSIGNED_CHAR:
					case Value::MOD8:
						n %= 8;
						mask <<= 7;
						k = 7;
						break;

					case Value::SIGNED_SHORT:
					case Value::UNSIGNED_SHORT:
					case Value::MOD16:
						n %= 16;
						mask <<= 15;
						k = 15;
						break;

					case Value::SIGNED_INT:
					case Value::UNSIGNED_INT:
					case Value::MOD32:
						n %= 32;
						mask <<= 31;
						k = 31;
						break;

					case Value::NO_TYPE:
						break;
				};

				for(int i=0; i<n; i++)
				{
					unsigned long long carry = (tmp & mask) >> k;
					tmp <<= 1;
					tmp |= carry;
				}

				pfArg -> setValue(tmp);
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::ROR:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isReadable() == false || pfArg -> isWriteable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			try
			{
				long long tmp = pfArg -> getValue();
				long long n = psArg -> getValue();
				unsigned long long mask = 1;
				int k;

				switch(pfArg -> getAfterCastType())
				{
					case Value::SIGNED_CHAR:
					case Value::UNSIGNED_CHAR:
					case Value::MOD8:
						n %= 8;
						k = 7;
						break;

					case Value::SIGNED_SHORT:
					case Value::UNSIGNED_SHORT:
					case Value::MOD16:
						n %= 16;
						k = 15;
						break;

					case Value::SIGNED_INT:
					case Value::UNSIGNED_INT:
					case Value::MOD32:
						n %= 32;
						k = 31;
						break;

					case Value::NO_TYPE:
						break;
				};

				for(int i=0; i<n; i++)
				{
					unsigned long long carry = (tmp & mask) << k;
					tmp >>= 1;
					tmp |= carry;
				}

				pfArg -> setValue(tmp);
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			m_contexts.top().m_ip++;
			break;
		}

		case Command::OR:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res;
			try
			{
				res = pfArg -> getValue();
				res |= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			pfArg -> setValue(res);

			m_contexts.top().m_ip++;

			break;
		}


		case Command::XOR:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res;
			try
			{
				res = pfArg -> getValue();
				res ^= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			pfArg -> setValue(res);

			m_contexts.top().m_ip++;

			break;
		}



		case Command::AND:
		{
			boost::shared_ptr<VarOperand> pfArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getFirstOperand());
			boost::shared_ptr<VarOperand> psArg = boost::dynamic_pointer_cast<VarOperand, Operand>
				(m_contexts.top().m_code[m_contexts.top().m_ip].getSecondOperand());

			if(pfArg -> hasValue() == false || psArg -> hasValue() == false)
				return Exception::ConstraintError;

			if(pfArg -> isWriteable() == false || pfArg -> isReadable() == false || psArg -> isReadable() == false)
				return Exception::ConstraintError;

			long long res;
			try
			{
				res = pfArg -> getValue();
				res &= static_cast<long long>(psArg -> getValue());
			}
			catch(std::runtime_error)
			{
				return Exception::ConstraintError;
			}

			pfArg -> setValue(res);

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




void CodeExecuter::setErrorStream(std::ostream &stream)
{
	m_perrStream = &stream;
}



void CodeExecuter::defaultExceptionHandler(Exception::Type except)
{
	*m_perrStream << "Program is interrupted with exception `" << Exception::exceptionTypeToStr(except) << "`"
	<< std::endl;
}
