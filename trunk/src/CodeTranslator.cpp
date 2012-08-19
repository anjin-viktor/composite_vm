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
//	std::cerr << "translator: 1\n";
	m_grammar.clear();
    std::string::iterator begin = str.begin(), end = str.end();

//	std::cerr << "translator: 2\n";

    bool success = qi::parse(begin, end, m_grammar);

//	std::cerr << "translator: 3\n";
/*
    if(m_command.getOperationType() != Command::NONE && m_command.getSecondOperand() != NULL 
    	&& m_command.getFirstOperand() != NULL)
    {
    	boost::shared_ptr<VarOperand> op2;
    	op2 = boost::dynamic_pointer_cast<VarOperand, Operand> (m_command.getSecondOperand());

//Если входит - значит второй операнд - константа
    	if(op2 -> getAfterCastType() == Value::NO_TYPE)
    	{
	    	boost::shared_ptr<VarOperand> op1 = boost::dynamic_pointer_cast<VarOperand, Operand> (m_command.getFirstOperand());

	    	if(op1 != NULL && op2 != NULL)
	    		op2 -> setType(op1 -> getAfterCastType());
    	}
    }
*/
    if(m_command.getOperationType() != Command::NONE)
    	checkCorrectness();

//	std::cerr << "translator: 4\n";


    if(!success || begin != end)
    {
		m_lbls.clear();
		m_command.setOperationType(Command::NONE);
		throw ParseError("stopped at: " + std::string(begin, end));
	}
//	std::cerr << "translator: 5\n";
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
				{
					std::cerr << op1 -> getAfterCastType() << ' ' << op2 -> getAfterCastType() << ' ' << op2 -> getType() << '\n';
					throw ParseError("mismatch");
				}
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

/*
		AOUT,
		RSZ,
		CALL,
		RET,
		NOP,
		CMP,
		NONE

*/





	};
}