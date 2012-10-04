#include "VarOperand.h"


#include <iostream>


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



bool VarOperand::hasValue() const
{
	if(m_pval != NULL)
		return true;
	else if(m_parr != NULL && m_indx < m_parr -> size())
		return true;
	else return false;
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




Value::ValueType VarOperand::getAfterCastType() const
{
	if(m_type != Value::NO_TYPE)
		return m_type;
	
	if(m_pval)
		return m_pval -> getType();
	else if(m_parr)
		return m_parr -> getType();

	return Value::NO_TYPE;
}




bool VarOperand::isCast() const
{
	return m_type != Value::NO_TYPE;
}



bool VarOperand::isReadable() const
{
	if(m_pval)
		return m_pval -> isReadable();

	if(m_parr)
		return m_parr -> operator[](m_indx).isReadable();

	return false;
}




void VarOperand::initialize()
{
	if(m_pval)
		m_pval -> setReadable(true);

	if(m_parr)
		m_parr -> operator[](m_indx).setReadable(true);
}




bool VarOperand::isWriteable() const
{
	if(m_const == true || m_type != Value::NO_TYPE)
		return false;

	if(m_pval)
		return m_pval -> isWriteable();

	if(m_parr)
		return m_parr -> operator[](m_indx).isWriteable();

	return false;
}




VarOperand VarOperand::convert(const DataKeeper *pold, DataKeeper *pnew) const
{
	VarOperand var;
	if(m_pval)
	{
		std::list<std::string> names = pold -> getValuesNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end() && 
			(!(pold -> getVarValue(*itr) == *m_pval) || (&(pold -> getVarValue(*itr)) != m_pval));
			 itr++);

		if(itr == names.end())
			return VarOperand();

		std::string name =  *itr;

		var.m_type = m_type;
		var.m_const = m_const;


		if(pnew -> isVar(name) == false)
			return VarOperand();

		var.setValuePtr(&(pnew -> getVarValue(name)));


		return var;
	}
	else if(m_parr)
	{
		std::list<std::string> names = pold -> getArraysNames();
		std::list<std::string>::const_iterator itr = names.begin();

		for(;itr != names.end() && 
			(!(pold -> getArray(*itr) == *m_parr) || (&pold -> getArray(*itr) != m_parr)); 
			itr++);

		if(itr == names.end())
			return VarOperand();

		std::string name =  *itr;
		var.m_type = m_type;
		var.m_const = m_const;


		if(pnew -> isArray(name) == false)
			return VarOperand();

		var.m_parr = &(pnew -> getArray(name));
		var.m_indx = m_indx;

		return var;
	}
	else return VarOperand();
}




void VarOperand::setValue(long long val)
{
	if(m_pval)
		m_pval -> setValue(val);
	else if(m_parr)
		return m_parr -> operator[](m_indx).setValue(val);
	else
		throw std::logic_error("setting value from NULL-object");	
}


void VarOperand::replace(Value *pold, Value *pnew)
{
	if(m_pval == pold)
		m_pval == pnew;
}


void VarOperand::replace(Array *pold, Array *pnew)
{
	if(m_parr == pold)
		m_parr == pnew;
}



bool VarOperand::canBeInit() const
{
	if(m_pval)
		return m_pval -> varCanBeInit();
	else if(m_parr)
		return m_parr -> operator[](m_indx).varCanBeInit();
	else
		throw std::logic_error("VarOperand is a NULL-object");	
}