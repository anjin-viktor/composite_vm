#include "CodeTranslator.h"


CodeTranslator::CodeTranslator()
{
	m_grammar.setCommandPtr(&m_command);
	m_grammar.setLabelNamesListPtr(&m_lbls);
}




CodeTranslator::~CodeTranslator()
{
}




void CodeTranslator::setDataKeeperPtr(DataKeeper *pkeeper)
{
	m_pdata = pkeeper;
	m_grammar.setDataKeeperPtr(pkeeper);
}




void CodeTranslator::translate(std::string str)
{
	m_grammar.clear();
    std::string::iterator begin = str.begin(), end = str.end();

    bool success = qi::parse(begin, end, m_grammar);


    if(!success || begin != end)
    {
		m_lbls.clear();
		m_command.setOperationType(Command::NONE);
		throw ParseError("stopped at: " + std::string(begin, end));
	}

    if(m_command.getOperationType() != Command::NONE)
    	checkCorrectness();
}




Command::Operation CodeTranslator::getOperation() const
{
	return m_command.getOperationType();
}



std::list<std::string> CodeTranslator::getLabelsList() const
{
	return m_lbls;
}




Command CodeTranslator::getCommand() const
{
	return m_command;
}





void CodeTranslator::checkCorrectness() const throw(ParseError)
{
	switch(m_command.getOperationType())
	{
		case Command::MOV:
		{
			boost::shared_ptr<VarOperand> op1, op2;
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());

/*Для отладки*/
			if(op1 -> getAfterCastType() != Value::NO_TYPE)
				if(op1 -> getAfterCastType() != op2 -> getAfterCastType() && op2 -> getAfterCastType() != Value::NO_TYPE)
					throw ParseError("mismatch");


			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op2 -> getAfterCastType() == Value::NO_TYPE)
					if(Value::isOverflow(op2 -> getValue(), op1 -> getAfterCastType()))
					{
						std::stringstream ss;
						ss << "first argument can't fit value " << op2 -> getValue();
						throw ParseError(ss.str());
					}


				if(op1 -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				if(op2 -> isReadable() == false && op2 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				op1 -> initialize();
			}
			break;
		}

		case Command::ADD:
		case Command::SUB:
		case Command::MUL:
		case Command::DIV:
		case Command::MOD:
		case Command::AND:
		case Command::OR:
		case Command::XOR:
		{
			boost::shared_ptr<VarOperand> op1, op2;
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());

/*Для отладки*/
			if(op1 -> getAfterCastType() != Value::NO_TYPE)
				if(op1 -> getAfterCastType() != op2 -> getAfterCastType() && op2 -> getAfterCastType() != Value::NO_TYPE)
					throw ParseError("mismatch");


			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op2 -> getAfterCastType() == Value::NO_TYPE)
					if(Value::isOverflow(op2 -> getValue(), op1 -> getAfterCastType()))
					{
						std::stringstream ss;
						ss << "first argument can't fit value " << op2 -> getValue();
						throw ParseError(ss.str());
					}


				if(op1 -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				if(op2 -> isReadable() == false && op2 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				if(op1 -> isReadable() == false && op1 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				op1 -> initialize();
			}
			break;
		}


		case Command::CMP:
		{
			boost::shared_ptr<VarOperand> op1, op2;
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());

			if(op1 -> getAfterCastType() != op2 -> getAfterCastType() &&  op2 -> getAfterCastType() != Value::NO_TYPE 
				&&  op1 -> getAfterCastType() != Value::NO_TYPE)
				throw ParseError("mismatch");

			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op1 -> isReadable() == false && op1 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				if(op2 -> isReadable() == false && op2 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");


			}

			break;
		}


		case Command::NOT:
		{
			boost::shared_ptr<VarOperand> op;
			op = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());

			if(op -> hasValue())
			{
				if(op -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				if(op -> isReadable() == false && op -> canBeInit() == false)
					throw ParseError("variable does not have read permission");
			}

			break;
		}

		case Command::JMP:
		case Command::JL:
		case Command::JE:
		case Command::JG:
		case Command::JNE:
		{

			break;
		}


		case Command::RSZ:
		{
			boost::shared_ptr<ArrayOperand> op1;
			boost::shared_ptr<VarOperand> op2;
		
			op1 = boost::dynamic_pointer_cast<ArrayOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());

			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op1 -> getArrayPtr() -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				if(op2 -> isReadable() == false && op2 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");
			}

			break;
		}


		case Command::AOUT:
		{
			boost::shared_ptr<ArrayOperand> op = boost::dynamic_pointer_cast<ArrayOperand, Operand>(m_command.getFirstOperand());

			if(op -> hasValue())
			{
				Array *parr = op -> getArrayPtr();

				if(parr -> size() && parr -> operator[](0).getType() != Value::MOD8 && parr -> operator[](0).getType() != Value::UNSIGNED_CHAR)
					throw ParseError("can not write an array");
			}
			
			break;
		}

		case Command::GTEL:
		{
			boost::shared_ptr<ArrayOperand> op2;
			boost::shared_ptr<VarOperand> op1;
			boost::shared_ptr<VarOperand> op3;


			op2 = boost::dynamic_pointer_cast<ArrayOperand, Operand>(m_command.getSecondOperand());
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op3 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getOperand(2));


			if(op1 -> hasValue() && op2 -> hasValue() && op3 -> hasValue())
			{
				if(op3 -> isReadable() == false && op3 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				if(op1 -> isWriteable() == false)
					throw ParseError("variable does not have write permission");


				if(op1 -> getAfterCastType() != Value::NO_TYPE && op1 -> getAfterCastType() != (op2 -> getArrayPtr() -> getType()))
					throw ParseError("mismatch");					
			
	 			op1 -> initialize();
			}			
 			

			break;
		}


		case Command::STEL:
		{
			boost::shared_ptr<ArrayOperand> op2;
			boost::shared_ptr<VarOperand> op1;
			boost::shared_ptr<VarOperand> op3;


			op2 = boost::dynamic_pointer_cast<ArrayOperand, Operand>(m_command.getSecondOperand());
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op3 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getOperand(2));


			if(op1 -> getAfterCastType() != Value::NO_TYPE)
				if(op1 -> getAfterCastType() != op2 -> getArrayPtr() -> getType())
					throw ParseError("mismatch");


			if(op1 -> hasValue() && op2 -> hasValue() && op3 -> hasValue())
			{
				if(op3 -> isReadable() == false && op3 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				if(op1 -> isReadable() == false && op1 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");
	
				if(op1 -> getAfterCastType() == Value::NO_TYPE)
					if(Value::isOverflow(op1 -> getValue(), op2 -> getArrayPtr() -> getType()))
					{
						std::stringstream ss;
						ss << "first argument can't fit value " << op1 -> getValue();
						throw ParseError(ss.str());
					}
			}	


			break;
		}

		case Command::SIZE:
		{
			boost::shared_ptr<ArrayOperand> op2;
			boost::shared_ptr<VarOperand> op1;

			op2 = boost::dynamic_pointer_cast<ArrayOperand, Operand>(m_command.getSecondOperand());
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());

			if(op1 -> hasValue())
			{
				if(op1 -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				op1 -> initialize();
			}

			break;
		}

		case Command::SHL:
		case Command::SHR:
		case Command::SHLWO:
		case Command::SHRWO:
		case Command::ROL:
		case Command::ROR:
		{
			boost::shared_ptr<VarOperand> op1, op2;
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());


			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op1 -> isReadable() == false && op1 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");

				if(op1 -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				if(op2 -> isReadable() == false && op2 -> canBeInit() == false)
					throw ParseError("variable does not have read permission");
			}

			break;
		}

		case Command::CALL:
		case Command::RET:
		case Command::NOP:
		case Command::NONE:
		break;

	};
}
