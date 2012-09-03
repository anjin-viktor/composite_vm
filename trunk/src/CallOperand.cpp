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
