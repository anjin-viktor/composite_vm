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

    if(m_command.getOperationType() != Command::NONE)
    	checkCorrectness();

    if(!success || begin != end)
    {
		m_lbls.clear();
		m_command.setOperationType(Command::NONE);
		throw ParseError("stopped at: " + std::string(begin, end));
	}
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
					throw ParseError("variable does not have read permission");

				if(op1 -> getAfterCastType() != op2 -> getAfterCastType())
					throw ParseError("mismatch");
			}

		}

/*		JMP,
		JL,
		JE,
		JG,
		JNE,
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