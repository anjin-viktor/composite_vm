#include "CallOperand.h"



CallOperand::CallOperand()
{
	m_parr = NULL;
}


CallOperand::~CallOperand()
{
}



void CallOperand::setValue(const Value &val)
{
	m_fisValue = true;
	m_val = val;
	m_parr = NULL;
}



void CallOperand::setArrayElement(Array *parr, std::size_t indx)
{
	m_fisValue = true;
	m_parr = parr;
	m_indx = indx;
}



void CallOperand::setArray(const Array &arr)
{
	m_fisValue = false;
	m_arr = arr;
	m_parr = NULL;
}




Value CallOperand::getValue() const
{
	if(m_parr == NULL)
		return m_val;
	else 
		return m_parr -> operator[](m_indx);
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




void CallOperand::setValueType(Value::ValueType type)
{
	m_val.setType(type);
}



CallOperand CallOperand::convert(const DataKeeper *pold, DataKeeper *pnew) const
{
	CallOperand op;
	if(m_fisValue)
	{
		std::list<std::string> names = pold -> getValuesNames();
		std::list<std::string>::const_iterator itr = names.begin();


		for(;itr != names.end() && !(pold -> getVarValue(*itr) == m_val); itr++);

		if(itr == names.end())
			return op;

		if(pnew -> isVar(*itr) == false)
			return op;

		op.setValue(pnew -> getVarValue(*itr));
		return op;
	}
	else
	{
		std::list<std::string> names = pold -> getArraysNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end() && !(pold -> getArray(*itr) == m_arr); itr++);

		if(itr == names.end())
			return op;

		if(pnew -> isArray(*itr) == false)
			return op;

		op.setArray(pnew -> getArray(*itr));
		return op;
	}

}



bool CallOperand::isArrayElement() const
{
	return m_parr != NULL;
}




Value::ValueType CallOperand::getValueType() const
{
	if(m_parr == NULL)
		return m_val.getType();
	else 
		return m_parr -> getType();
}
