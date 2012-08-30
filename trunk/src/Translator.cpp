#include "Translator.h"


Translator::Translator(std::string fname)
{
	setInputFileName(fname);
}


Translator::~Translator()
{
}




void Translator::setInputFileName(const std::string &fname)
{
	m_in.close();
	m_inputFileName = fname;
}




std::string Translator::getInputFileName() const
{
	return m_inputFileName;
}




bool Translator::readString(std::string &str)
{
	m_lineNumb++;
	char endLineChar;

	if(m_in.is_open() == false)
		m_in.open(m_inputFileName.c_str(), std::ifstream::in);

	if(m_in.is_open() == false)
		throw std::runtime_error("file " + m_inputFileName + " not exists");


	if(m_in.eof() == true)
		return true;

	std::getline(m_in, str);


	return m_in.eof();
}



bool Translator::isEmptyOrComment(const std::string &str)
{
	return str.length() == 0 || boost::istarts_with(str, ";");
}



void Translator::translate()
{
	m_lineNumb = 0;
	Program::getInstance().clear();
	std::string str;

	while(readString(str) == false)
	{
		boost::trim(str);
		if(Translator::isEmptyOrComment(str) == false)
		{
			if(boost::istarts_with(str, ".name") == false)
				throw ParseError(str + " not expected");

			translateFunction(str);
	
		}
	}

	callOperandsCheck();

	m_in.close();
}





void Translator::translateFunction(const std::string &header)
{
	std::string str, funcName;
	try
	{
		funcName = HeaderTranslator::getNameFromStr(header);
	}
	catch(std::runtime_error err)
	{
		throw ParseError("header " + header + " is incorrect");
	}

	Function func;
	func.setName(funcName);

	bool fError = true;
	bool fEnd = false;
	while(readString(str) == false && fEnd == false)
	{
		boost::trim(str);
		if(Translator::isEmptyOrComment(str) == false)
		{
			if(str != ".var")
				throw ParseError(str + " not expected");
			else
			{
				fError = false;
				fEnd = true;
			}
		}
	}


	if(fError)
		throw ParseError("unexpected end of file");

	VarTranslator varTransl;
	fEnd = false;

	do
	{
		boost::trim(str);
		if(str == ".begin")
			fEnd = true;
		else 
			if(Translator::isEmptyOrComment(str) == false && fEnd == false)
				varTransl.translate(str);
	}
	while(fEnd == false && readString(str) == false);



	if(str != ".begin")
	{
		if(str == "")		
			throw ParseError("unexpected end of file");
		else
			throw ParseError(str + " not expected");
	}

	fEnd = false;

	Program::getInstance().addFunction(func);
	Program::getInstance().getFunction(funcName).setDataKeeper(varTransl.getDataKeeper());

	HeaderTranslator headerTransl;
	headerTransl.setDataKeeperPtr(Program::getInstance().getFunction(funcName).getDataKeeperPtr());
	headerTransl.translate(header);

	std::list<std::string> argNames = headerTransl.getArgsNames();
	std::list<std::string>::const_iterator itr = argNames.begin();

	for(;itr != argNames.end(); itr++)
		func.argIsRef(*itr, headerTransl.argIsRef(*itr));

	Program::getInstance().getFunction(funcName).setArgsNamesFromList(headerTransl.getArgsNames());

	CodeBlockTranslator codeTransl;
	codeTransl.setDataKeeperPtr(Program::getInstance().getFunction(funcName).getDataKeeperPtr());

	while(readString(str) == false && fEnd == false)
	{
		boost::trim(str);
		if(boost::istarts_with(str, ".end") || boost::istarts_with(str, ".exception"))
			fEnd = true;

		if(Translator::isEmptyOrComment(str) == false && fEnd == false)
			codeTransl.translate(str, m_lineNumb);
	}

	if(fEnd == false)
	{
		throw ParseError("unexpected end of file");
	}

	Program::getInstance().getFunction(funcName).setCommands(codeTransl.getCommands());

}




void Translator::callOperandsCheck() const
{
	std::list<std::string> lstNames = Program::getInstance().getFunctionNames();
	std::list<std::string>::const_iterator itr = lstNames.begin();

	for(;itr != lstNames.end(); itr++)
	{
		std::vector<Command> code = Program::getInstance().getFunction(*itr).getCommands();

		for(int i=0, size=code.size(); i<size; i++)
		{
			if(code[i].getOperationType() == Command::CALL)
			{
				std::string callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(code[i].getFirstOperand()) -> getLabelName();

				std::list<std::string>::const_iterator itrNames = std::find(lstNames.begin(), lstNames.end(), callName);

				if(itrNames == lstNames.end())
				{
					throw ParseError("function with name " + callName + " not exists");
				}

				std::list<std::string> funcArgsNames = Program::getInstance().getFunction(callName).getArgsNames();

/**
Command::getNumberOfOperands() имеет смысл только при числе операндов >= 2
*/
				if(funcArgsNames.size() != (code[i].getNumberOfOperands() - 1))
				{

					if(funcArgsNames.size() != 0)
					{
						throw ParseError("wrong number of operands");						
					}


					if(code[i].getSecondOperand().get() != NULL)
					{
						throw ParseError("wrong number of operands");
					}
				}
				else if(funcArgsNames.size() == 1 && code[i].getSecondOperand().get() == NULL)
				{
					throw ParseError("wrong number of operands");
				}

				itrNames = funcArgsNames.begin();

				for(int j=0; itrNames != funcArgsNames.end(); itrNames++, j++)
				{
					boost::shared_ptr<CallOperand> pop =  boost::dynamic_pointer_cast<CallOperand, Operand>
						(code[i].getOperand(j+1));

/*Это константа*/
					if(pop -> isValue() && pop -> getValue().getType() == Value::NO_TYPE
						&& Program::getInstance().getFunction(callName).getDataKeeperPtr() -> isVar(*itrNames))
					{
						pop -> setValueType(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).getType());
					}

					if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> isVar(*itrNames))
					{
						if(pop -> isValue() == false)
						{
							throw ParseError("incorrect operand type in call function " + callName);
						}
						if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).getType() !=
							pop -> getValue().getType() /*||
							(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).isWriteable()
							&& pop -> getValue().isWriteable() == false)*/
						  )
						{
							throw ParseError("incorrect operand type in call function " + callName);
						}
					}
					else
					{
						if(pop -> isArray() == false)
						{

							throw ParseError("incorrect operand type in call function " + callName);
						}


						if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getArray(*itrNames).getType() !=
							pop -> getArray().getType() /*||
							(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getArray(*itrNames).isWriteable()
							&& pop -> getArray().isWriteable() == false)*/
						  )
						{

							throw ParseError("incorrect operand type in call function " + callName);
						}

					}
				}
			}
		}
	}
}
