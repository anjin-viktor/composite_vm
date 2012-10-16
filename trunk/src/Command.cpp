#include "Command.h"


Command::Command()
{
	m_opType = Command::NONE;
	m_operands.resize(2);
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



void Command::setOperand(std::size_t n, boost::shared_ptr<Operand> poperand)
{
	if(m_operands.size() < n+1)
		m_operands.resize(n+1);

	m_operands[n] = poperand;
}



boost::shared_ptr<Operand> Command::getFirstOperand() const throw(std::out_of_range)
{
	return m_operands[0];
}




boost::shared_ptr<Operand> Command::getSecondOperand() const throw(std::out_of_range)
{
	return m_operands[1];
}



boost::shared_ptr<Operand> Command::getOperand(std::size_t n) const throw(std::out_of_range)
{
	return m_operands[n];
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
	else if(tmp == "aout")
		return Command::AOUT;
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
	else if(tmp == "gtel")
		return Command::GTEL;
	else if(tmp == "stel")
		return Command::STEL;
	else if(tmp == "size")
		return Command::SIZE;
	else if(tmp == "shr")
		return Command::SHR;
	else if(tmp == "shl")
		return Command::SHL;
	else if(tmp == "shrwo")
		return Command::SHRWO;
	else if(tmp == "shlwo")
		return Command::SHLWO;
	else if(tmp == "ror")
		return  Command::ROR;
	else if(tmp == "rol")
		return Command::ROL;
	else if(tmp == "and")
		return Command::AND;
	else if(tmp == "or")
		return Command::OR;
	else if(tmp == "xor")
		return Command::XOR;
	else
		return Command::NONE;
}




std::string Command::operationToStr(Command::Operation op)
{
	std::string str;
	switch(op)
	{
		case Command::MOV:
			str = "MOV";
			break;
		case Command::ADD:
			str = "ADD";
			break;
		case Command::SUB:
			str = "SUB";
			break;
		case Command::MUL:
			str = "MUL";
			break;
		case Command::DIV:
			str = "DIV";
			break;
		case Command::MOD:
			str = "MOD";
			break;
		case Command::JMP:
			str = "JMP";
			break;
		case Command::JL:
			str = "JL";
			break;
		case Command::JE:
			str = "JE";
			break;
		case Command::JG:
			str = "JG";
			break;
		case Command::JNE:
			str = "JNE";
			break;
		case Command::AOUT:
			str = "AOUT";
			break;
		case Command::RSZ:
			str = "RSZ";
			break;
		case Command::CALL:
			str = "CALL";
			break;
		case Command::RET:
			str = "RET";
			break;
		case Command::NOP:
			str = "NOP";
			break;
		case Command::CMP:
			str = "CMP";
			break;
		case Command::GTEL:
			str = "GTEL";
			break;
		case Command::STEL:
			str = "STEL";
			break;
		case Command::SIZE:
			str = "SIZE";
			break;
		case Command::SHL:
			str = "SHL";
			break;
		case Command::SHR:
			str = "SHR";
			break;
		case Command::SHRWO:
			str = "SHRWO";
			break;
		case Command::SHLWO:
			str = "SHLWO";
			break;
		case Command::ROR:
			str = "ROR";
			break;
		case Command::ROL:
			str = "ROL";
			break;
		case Command::AND:
			str = "AND";
			break;
		case Command::OR:
			str = "OR";
			break;
		case Command::XOR:
			str = "XOR";
			break;
		case Command::NONE:
			str = "NONE";
			break;
	};
	return str;
}





void Command::setLineNumber(std::size_t numb)
{
	m_numb = numb;
}



std::size_t Command::getLineNumber() const
{
	return m_numb;
}



std::ostream &operator << (std::ostream &stream, const Command &comm)
{
	stream << Command::operationToStr(comm.m_opType) << "(" << comm.m_numb << ")";
	return stream;
}




bool Command::operator !=(const Command &command)
{
	return m_opType != command.m_opType || m_numb != command.m_numb;
}




std::size_t Command::getNumberOfOperands() const
{
	return m_operands.size();
}
