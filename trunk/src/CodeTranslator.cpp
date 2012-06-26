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