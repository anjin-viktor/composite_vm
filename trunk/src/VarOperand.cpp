#include "VarOperand.h"

VarOperand::VarOperand(): Operand()
{
}




VarOperand::VarOperand(const std::string &name) throw(std::runtime_error): Operand()
{
	setVarName(name);
}




VarOperand::VarOperand(long long value): Operand()
{
	setLongLongValue(value);
}




VarOperand::~VarOperand()
{
}




void VarOperand::setVarName(const std::string &name) throw(std::runtime_error)
{
//если переменной с именем name нет - выкинуть исключение
//установить m_pval на переменную с именем name 
}




void VarOperand::setLongLongValue(long long value)
{
//Если в рамках функции нет константы с этим именем - добавить
//установить m_pval на переменную с этим значением
}




void VarOperand::setType(Value::ValueType type)
{
	m_type = type;
}
