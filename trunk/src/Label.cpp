#include "Label.h"

Label::Label(): Operand()
{
}




Label::Label(const std::string &label_name): Operand()
{
	m_labelName = label_name;
}




Label::~Label()
{
}




void Label::setLabelName(const std::string &name)
{
	if(Label::checkLabelName(name))
		m_labelName = name;
	else
		throw std::logic_error("Incorrect label name");
}




std::string Label::getLabelName() const
{
	return m_labelName;
}





void Label::setCommandPtr()
{
}




void Label::getCommandPtr()
{
}




bool Label::checkLabelName(const std::string &labelName)
{
	boost::regex expr("[a-zA-Z_][a-zA-Z0-9_]*");
    boost::cmatch what;

    return boost::regex_match(labelName.c_str(), what, expr);
}