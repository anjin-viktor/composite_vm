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
		case Command::ADD:
		case Command::SUB:
		case Command::MUL:
		case Command::DIV:
		case Command::MOD:
		{
			boost::shared_ptr<VarOperand> op1, op2;
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());

			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op1 -> isWriteable() == false)
					throw ParseError("variable does not have write permission");

				if(op2 -> isReadable() == false)
					throw ParseError("1variable does not have read permission");

				if(op1 -> getAfterCastType() != op2 -> getAfterCastType() && op2 -> getAfterCastType() != Value::NO_TYPE)
					throw ParseError("mismatch");

				op1 -> initialize();
			}
			break;
		}


		case Command::CMP:
		{
			boost::shared_ptr<VarOperand> op1, op2;
			op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getFirstOperand());
			op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(m_command.getSecondOperand());


			if(op1 -> hasValue() && op2 -> hasValue())
			{
				if(op1 -> isReadable() == false)
					throw ParseError("variable does not have read permission");

				if(op2 -> isReadable() == false)
					throw ParseError("variable does not have read permission");

				if(op1 -> getAfterCastType() != op2 -> getAfterCastType() &&  op2 -> getAfterCastType() != Value::NO_TYPE 
					&&  op1 -> getAfterCastType() != Value::NO_TYPE)
					throw ParseError("mismatch");
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

				if(op2 -> isReadable() == false)
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

				for(int i=0; i<parr -> size(); i++)
					if(parr -> operator[](i).isReadable() == false)
						throw ParseError("variable does not have read permission");
			}
			
			break;
		}

/*
		CALL,
		RET,
		NOP,
		NONE
*/
	};
}