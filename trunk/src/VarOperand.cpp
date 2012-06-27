#include "VarOperand.h"

VarOperand::VarOperand(Value *pval, Value::ValueType type): Operand()
{
	m_pval = pval;
	m_type = type;
	m_parr = NULL;
	m_indx = 0;
	m_const = false;
}



VarOperand::VarOperand(Array *parr, std::size_t n, Value::ValueType type): Operand()
{
	m_pval = NULL;
	m_type = type;
	m_parr = parr;
	m_indx = n;
	m_const = false;
}




VarOperand::~VarOperand()
{
}




void VarOperand::setValuePtr(Value *pval)
{
	m_pval = pval;
	m_parr = NULL;
	m_indx = 0;
}




void VarOperand::setArrayElementPtr(Array *parr, std::size_t n)
{
	m_pval = NULL;
	m_parr = parr;
	m_indx = n;
}



void VarOperand::setType(Value::ValueType type)
{
	m_type = type;
}




long long VarOperand::getValue() const
{
	if(m_pval)
		return m_pval -> getValue(m_type);
	else if(m_parr)
		return m_parr -> operator[](m_indx).getValue(m_type);
	else
		throw std::logic_error("getting value from NULL-object");

}




void VarOperand::setConstancy(bool constancy)
{
	m_const = constancy;
}




bool VarOperand::isConstant() const
{
	return m_const;
}




Value::ValueType VarOperand::getType() const
{
	return m_type;
}




bool VarOperand::isCast() const
{
	return m_type != Value::NO_TYPE;
}