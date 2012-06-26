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




void Command::setFirstOperand(boost::shared_ptr<Operand> poperand)
{
	m_operands[0] = poperand;
}




void Command::setSecondOperand(boost::shared_ptr<Operand> poperand)
{
	m_operands[1] = poperand;
}





boost::shared_ptr<Operand> Command::getFirstOperand() const throw(std::out_of_range)
{
	return m_operands[0];
}




boost::shared_ptr<Operand> Command::getSecondOperand() const throw(std::out_of_range)
{
	return m_operands[1];
}




Command::Operation Command::strToOperation(const std::string &str)
{
	std::string tmp = str;
	boost::algorithm::to_lower(tmp);

	if(tmp == "mov")
		return Command::MOV;
	else if(tmp == "add")
		return Command::ADD;
	else if(tmp == "sub")
		return Command::SUB;
	else if(tmp == "mul")
		return Command::MUL;
	else if(tmp == "div")
		return Command::DIV;
	else if(tmp == "mod")
		return Command::MOD;
	else if(tmp == "jmp")
		return Command::JMP;
	else if(tmp == "jl")
		return Command::JL;
	else if(tmp == "je")
		return Command::JE;
	else if(tmp == "jg")
		return Command::JG;
	else if(tmp == "jne")
		return Command::JNE;
	else if(tmp == "out")
		return Command::OUT;
	else if(tmp == "rsz")
		return Command::RSZ;
	else if(tmp == "call")
		return Command::CALL;
	else if(tmp == "ret")
		return Command::RET;
	else if(tmp == "nop")
		return Command::NOP;
	else if(tmp == "cmp")
		return Command::CMP;
	else 
		return Command::NONE;
}
