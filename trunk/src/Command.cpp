#include "Command.h"


Command::Command()
{
	m_operands.resize(2);
	m_opType = Command::NONE;
}




Command::Command(Command::Operation operation)
{
	m_opType = operation;
	m_operands.resize(2);
}




Command::~Command()
{
}




void Command::setOperationType(Command::Operation operation)
{
	m_opType = operation;
}




Command::Operation Command::getOperationType() const
{
	return m_opType;
}




void Command::setFirstOperand(const Operand &operand)
{
	m_operands[0] = operand;
}




void Command::setSecondOperand(const Operand &operand)
{
	m_operands[1] = operand;
}
