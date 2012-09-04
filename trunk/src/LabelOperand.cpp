#include "LabelOperand.h"

LabelOperand::LabelOperand(): Operand()
{
}




LabelOperand::LabelOperand(const std::string &label_name): Operand()
{
	m_labelName = label_name;
}




LabelOperand::~LabelOperand()
{
}




void LabelOperand::setLabelName(const std::string &name)
{
	if(LabelOperand::checkLabelName(name))
		m_labelName = name;
	else
		throw std::logic_error("Incorrect label name");
}




std::string LabelOperand::getLabelName() const
{
	return m_labelName;
}





void LabelOperand::setCommandOffset(std::size_t offset)
{
	m_offset = offset;
}




std::size_t LabelOperand::getCommandOffset() const
{
	return m_offset;
}




bool LabelOperand::checkLabelName(const std::string &labelName)
{
	boost::regex expr("[a-zA-Z_][a-zA-Z0-9_]*");
    boost::cmatch what;

    return boost::regex_match(labelName.c_str(), what, expr);
}
