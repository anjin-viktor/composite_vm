#include "ArrayOperand.h"


ArrayOperand::ArrayOperand(Array *parr)
{
	m_parr = parr;
}




ArrayOperand::~ArrayOperand()
{
}




void ArrayOperand::setArrayPtr(Array *parr)
{
	m_parr = parr;
}




Array *ArrayOperand::getArrayPtr() const
{
	return m_parr;
}




bool ArrayOperand::hasValue() const
{
	return m_parr != NULL;
}