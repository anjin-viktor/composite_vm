#include "CallOperand.h"


CallOperand::CallOperand()
{
}


CallOperand::~CallOperand()
{
}



void CallOperand::setValue(const Value &val)
{
	m_fisValue = true;
	m_val = val;
}




void CallOperand::setArray(const Array &arr)
{
	m_fisValue = false;
	m_arr = arr;
}




Value CallOperand::getValue() const
{
	return m_val;
}




Array CallOperand::getArray() const
{
	return m_arr;
}



bool CallOperand::isValue() const
{
	return m_fisValue;
}



bool CallOperand::isArray() const
{
	return !m_fisValue;
}
