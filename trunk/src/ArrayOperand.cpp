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



ArrayOperand ArrayOperand::convert(const DataKeeper *pold, DataKeeper *pnew) const
{
	if(m_parr)
	{
		std::list<std::string> names = pold -> getArraysNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end()&& !(pold -> getArray(*itr) == *m_parr); itr++);

		if(itr == names.end())
			return ArrayOperand();


		if(pnew -> isArray(*itr) == false)
			return ArrayOperand();

		ArrayOperand arr;
		arr.m_parr = &(pnew -> getArray(*itr));

		return arr;
	}
	else
		return ArrayOperand();
}


void ArrayOperand::replace(Array *pold, Array *pnew)
{
	if(m_parr == pold)
		m_parr = pnew;	
}
