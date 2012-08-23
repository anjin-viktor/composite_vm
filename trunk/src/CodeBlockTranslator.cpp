#include "CodeBlockTranslator.h"


CodeBlockTranslator::CodeBlockTranslator()
{
}



CodeBlockTranslator::CodeBlockTranslator(DataKeeper *pdata)
{
	setDataKeeperPtr(pdata);
}



CodeBlockTranslator::~CodeBlockTranslator()
{
}



void CodeBlockTranslator::setDataKeeperPtr(DataKeeper *pdata)
{
	m_codeTranslator.setDataKeeperPtr(pdata);
}



void CodeBlockTranslator::translate(const std::string &str, std::size_t lineNo)
{
	m_codeTranslator.translate(str);


	std::list<std::string> lst = m_codeTranslator.getLabelsList();

	std::list<std::string>::const_iterator itr = lst.begin();

	for(; itr != lst.end(); itr++)
		m_mapLabels[m_vcommands.size()].push_back(*itr);

	if(m_codeTranslator.getOperation() != Command::NONE)
	{
		Command c = m_codeTranslator.getCommand();
		c.setLineNumber(lineNo);
		m_vcommands.push_back(c);
	}
}



std::vector<Command> CodeBlockTranslator::getCommands()
{
	processJmpCommands();
	return m_vcommands;
}




void CodeBlockTranslator::clear()
{
	m_vcommands.clear();
	m_mapLabels.clear();
}



void CodeBlockTranslator::processJmpCommands()
{
	for(int i=0; i<m_vcommands.size(); i++)
	{
		Command::Operation op = m_vcommands[i].getOperationType();
		if(op == Command::JMP || op == Command::JG || op == Command::JL || op == Command::JE || op == Command::JNE)
		{
			std::string lblName = (boost::dynamic_pointer_cast<LabelOperand, Operand>(m_vcommands[i].getFirstOperand())) -> getLabelName();

			bool flblExists = false;
			for(int j=0; j<m_vcommands.size(); j++)
			{
				std::map<int, std::list<std::string> >::iterator itr = m_mapLabels.begin();
				bool fstop = false;
				for(;itr != m_mapLabels.end() && fstop == false; itr++)
					if(find((itr -> second).begin(), (itr -> second).end(), lblName) != (itr -> second).end())
					{
						fstop = true;
						flblExists = true;
						(boost::dynamic_pointer_cast<LabelOperand, Operand>(m_vcommands[i].getFirstOperand())) -> setCommandOffset(itr -> first);
					}
			}

			if(flblExists == false)
				throw ParseError(std::string("Error at line ") /* добавить номер строки */ + ": label with name \"" + lblName +"\" not exists");
		}
	}
}
