#include "Label.h"

Label::Label()
{
}



Label::Label(const std::string &strName, std::size_t addr) throw(std::logic_error)
{
	setLabelName(strName);
	m_addr = addr;
}



Label::~Label()
{
}




void Label::setLabelName(const std::string &strName) throw(std::logic_error) 
{
	boost::regex expr("[a-zA-Z_][a-zA-Z0-9_]*");
    boost::cmatch what;

    if(boost::regex_match(strName.c_str(), what, expr))
    	m_labelName = strName;
    else
    	throw std::logic_error("Incorrect label name + " + strName);
}



void Label::setLabelAddr(std::size_t addr)
{
	m_addr = addr;
}




std::string Label::getLabelName() const
{
	return m_labelName;
}



std::size_t Label::getLabelAddr() const
{
	return m_addr;
}
