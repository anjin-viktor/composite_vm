#include "CallOperand.h"



CallOperand::CallOperand()
{
	m_pval = NULL;
	m_popArr = NULL;
	m_parr = NULL;
	m_type = Value::NO_TYPE;
	m_writeable = true;
}


CallOperand::~CallOperand()
{
}



void CallOperand::setValuePtr(Value *pval)
{
	m_pval = pval;
	m_parr = NULL;
	m_popArr = NULL;
}



void CallOperand::setArrayElement(Array *parr, std::size_t indx)
{
	m_parr = parr;
	m_indx = indx;
	m_pval = NULL;
	m_popArr = NULL;
}



void CallOperand::setArrayPtr(Array *parr)
{
	m_popArr = parr;
	m_parr = NULL;
	m_pval = NULL;
}




Value CallOperand::getValue() const
{
	if(m_parr == NULL)
		return *m_pval;
	else 
		return m_parr -> operator[](m_indx);
}




Array CallOperand::getArray() const
{
	assert(m_popArr != NULL);
	return *m_popArr;
}



bool CallOperand::isValue() const
{
	return m_pval != NULL || m_parr != NULL;;
}



bool CallOperand::isArray() const
{
	return m_popArr != NULL;
}




void CallOperand::setValueType(Value::ValueType type)
{
	m_type = type;
}



CallOperand CallOperand::convert(const DataKeeper *pold, DataKeeper *pnew) const
{
	CallOperand op;

	if(m_pval != NULL)
	{
		std::list<std::string> names = pold -> getValuesNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end() && !(&(pold -> getVarValue(*itr)) == m_pval); itr++);

		assert(itr != names.end());
		assert(pnew -> isVar(*itr) != false);

		if(itr == names.end())
			return op;
			
		if(pnew -> isVar(*itr) == false)
			return op;

		op.m_pval = &(pnew -> getVarValue(*itr));
		return op;
	}
	else if(m_parr != NULL)
	{
		std::list<std::string> names = pold -> getArraysNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end() && !(&(pold -> getArray(*itr)) == m_parr); itr++);

		assert(itr != names.end());
		assert(pnew -> isArray(*itr) != false);

		if(itr == names.end())
			return op;

		if(pnew -> isArray(*itr) == false)
			return op;

		op.m_parr = &(pnew -> getArray(*itr));
		return op;	
	}
	else if(m_popArr)
	{
		std::list<std::string> names = pold -> getArraysNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end() && !(&(pold -> getArray(*itr)) == m_popArr); itr++);
	

		assert(itr != names.end());
		assert(pnew -> isArray(*itr) != false);

		if(itr == names.end())
			return op;

		if(pnew -> isArray(*itr) == false)
			return op;

		op.m_popArr = &(pnew -> getArray(*itr));
		return op;
	}
}



bool CallOperand::isArrayElement() const
{
	return m_parr != NULL;
}




Value::ValueType CallOperand::getValueType() const
{
	if(m_type == Value::NO_TYPE)
	{
		if(m_parr == NULL)
			return m_pval -> getType();
		else 
			return m_parr -> getType();
	}
	else
		return m_type;
}



void CallOperand::replace(Array *pold, Array *pnew)
{
	if(m_parr == pold)
		m_parr = pnew;

	if(m_popArr == pold)
		m_popArr = pnew;
}



void CallOperand::setWriteable(bool writeable)
{
	m_writeable = writeable;
}




bool CallOperand::hasValue() const
{
	if(m_pval)
		return true;
	else if(m_popArr)
		return true;
	else if(m_parr)
		return m_indx < m_parr -> size();
	else
		return false;
}



bool CallOperand::isWriteable() const
{
	if(m_writeable == false)
		return m_writeable;
	else
	{
		if(m_pval)
			return m_pval -> isWriteable();
		else if(m_parr)
			return m_parr -> operator[](m_indx).isWriteable();
		else if(m_popArr)
			return m_popArr -> isWriteable();
		else
			return false;
	}
}