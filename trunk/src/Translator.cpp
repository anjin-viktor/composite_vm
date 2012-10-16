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
	TranslatedFileInfo::getInstance().setFile(fname);
	TranslatedFileInfo::getInstance().clearLineNo();
}




std::string Translator::getInputFileName() const
{
	return m_inputFileName;
}




bool Translator::readString(std::string &str)
{
	m_lineNumb++;
	char endLineChar;

	TranslatedFileInfo::getInstance().incLineNo();

	if(m_in.is_open() == false)
		m_in.open(m_inputFileName.c_str(), std::ifstream::in);

	if(m_in.is_open() == false)
		throw std::runtime_error("file `" + m_inputFileName + "` not exists");


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

	if(Program::getInstance().functionIsExists("main") == false)
		throw ParseError("function `main` not exists");

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
		std::size_t pos = str.find(";");

		if(pos != std::string::npos)
			str.erase(pos);

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


	Program::getInstance().getFunction(funcName).setArgsNamesFromList(headerTransl.getArgsNames());

	for(;itr != argNames.end(); itr++)
		Program::getInstance().getFunction(funcName).argIsRef(*itr, headerTransl.argIsRef(*itr));


	CodeBlockTranslator codeTransl;
	codeTransl.setDataKeeperPtr(Program::getInstance().getFunction(funcName).getDataKeeperPtr());

	while(fEnd == false && readString(str) == false)
	{
		boost::trim(str);
		if(boost::istarts_with(str, ".end") || boost::istarts_with(str, ":when_"))
			fEnd = true;

		if(Translator::isEmptyOrComment(str) == false && fEnd == false)
			codeTransl.translate(str, m_lineNumb);
	}

	if(fEnd == false)
	{
		throw ParseError("unexpected end of file");
	}

	Program::getInstance().getFunction(funcName).setCommands(codeTransl.getCommands());


	for(; boost::istarts_with(str, ":when_");)
	{
		str = translateExceptionHandler(str, funcName);
		boost::trim(str);
	}


	if(boost::istarts_with(str, ".end") == false)
	{
		throw ParseError("unexpected end of file");
	}

	if(codeTransl.getCommands().empty())
	{
		throw ParseError("function `" + funcName + "` has empty body");
	}
}




void Translator::callOperandsCheck() const
{
	std::list<std::string> lstNames = Program::getInstance().getFunctionNames();
	std::list<std::string>::const_iterator itr = lstNames.begin();

	for(;itr != lstNames.end(); itr++)
	{
		std::vector<Command> code = Program::getInstance().getFunction(*itr).getCommands();

		for(int i=0, size=code.size(); i<size; i++)
			if(code[i].getOperationType() == Command::CALL)
				callCheck(code[i]);

		if(Program::getInstance().getFunction(*itr).exceptionHandlerIsExists(Exception::NumericError))
		{
			code = Program::getInstance().getFunction(*itr).getExceptionHandlerCode(Exception::NumericError);

			for(int i=0, size=code.size(); i<size; i++)
				if(code[i].getOperationType() == Command::CALL)
					callCheck(code[i]);
		}

		if(Program::getInstance().getFunction(*itr).exceptionHandlerIsExists(Exception::ConstraintError))
		{
			code = Program::getInstance().getFunction(*itr).getExceptionHandlerCode(Exception::ConstraintError);

			for(int i=0, size=code.size(); i<size; i++)
				if(code[i].getOperationType() == Command::CALL)
					callCheck(code[i]);
		}

		if(Program::getInstance().getFunction(*itr).exceptionHandlerIsExists(Exception::ProgramError))
		{
			code = Program::getInstance().getFunction(*itr).getExceptionHandlerCode(Exception::ProgramError);

			for(int i=0, size=code.size(); i<size; i++)
				if(code[i].getOperationType() == Command::CALL)
					callCheck(code[i]);
		}

		if(Program::getInstance().getFunction(*itr).exceptionHandlerIsExists(Exception::StorageError))
		{
			code = Program::getInstance().getFunction(*itr).getExceptionHandlerCode(Exception::StorageError);

			for(int i=0, size=code.size(); i<size; i++)
				if(code[i].getOperationType() == Command::CALL)
					callCheck(code[i]);
		}
	}
}






void Translator::callCheck(Command command) const
{
	std::list<std::string> lstNames = Program::getInstance().getFunctionNames();
	std::string callName = boost::dynamic_pointer_cast<LabelOperand, Operand>(command.getFirstOperand()) -> getLabelName();
	std::list<std::string>::const_iterator itrNames = std::find(lstNames.begin(), lstNames.end(), callName);


	if(itrNames == lstNames.end())
		throw ParseError("function with name `" + callName + "` not exists");

	std::list<std::string> funcArgsNames = Program::getInstance().getFunction(callName).getArgsNames();

/**
Command::getNumberOfOperands() имеет смысл только при числе операндов >= 2
*/
	if(funcArgsNames.size() != (command.getNumberOfOperands() - 1))
	{
		if(funcArgsNames.size() != 0)
			throw ParseError("wrong number of operands");						


		if(command.getSecondOperand().get() != NULL)
			throw ParseError("wrong number of operands");
	}
	else if(funcArgsNames.size() == 1 && command.getSecondOperand().get() == NULL)
		throw ParseError("wrong number of operands");

	itrNames = funcArgsNames.begin();

	for(int j=0; itrNames != funcArgsNames.end(); itrNames++, j++)
	{
		boost::shared_ptr<CallOperand> pop =  boost::dynamic_pointer_cast<CallOperand, Operand>
		(command.getOperand(j+1));

/*Это константа*/
		if(pop -> isArrayElement() == false && pop -> isValue() && pop -> getValue().getType() == Value::NO_TYPE
			&& Program::getInstance().getFunction(callName).getDataKeeperPtr() -> isVar(*itrNames))
				pop -> setValueType(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).getType());

		if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> isVar(*itrNames))
		{
			if(pop -> isValue() == false)
				throw ParseError("incorrect operand type in call function`" + callName + "`");

			if(pop -> isArrayElement() == false)
			{
				if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).getType() !=
						pop -> getValueType())
					throw ParseError("incorrect operand type in call function `" + callName + "`");
		
				if(Program::getInstance().getFunction(callName).argIsRef(*itrNames) && 
			    	Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).isWriteable() == true &&
			    	 pop -> isWriteable() == false
			  	)
					throw ParseError("incorrect operand type in call function `" + callName + "`");
			}
			else if(pop -> hasValue())
			{
				if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getVarValue(*itrNames).getType() !=
						pop -> getValueType())
					throw ParseError("incorrect operand type in call function `" + callName + "`");
			}

		}
		else
		{
			if(pop -> isArray() == false)
				throw ParseError("incorrect operand type in call function " + callName);

			if(Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getArray(*itrNames).getType() !=
				pop -> getArray().getType())
				throw ParseError("incorrect operand type in call function " + callName);


			if(Program::getInstance().getFunction(callName).argIsRef(*itrNames) && 
			    Program::getInstance().getFunction(callName).getDataKeeperPtr() -> getArray(*itrNames).isWriteable() == true &&
			     pop -> getArray().isWriteable() == false
			  )
				throw ParseError("incorrect operand type in call function " + callName);
		}
	}
}


std::string Translator::translateExceptionHandler(const std::string &header, const std::string &funcName)
{
	boost::regex cmpExpr("^\\s*\\:when_[\\l\\u_]*\\s*(;.*)?");
    boost::regex expr(":when_[\\l\\u_]*");
 
    boost::smatch res;
    std::string::const_iterator start = header.begin();
    std::string::const_iterator end = header.end();

    std::string exceptionType;

    if(boost::regex_match(start, end, res, cmpExpr))
    {
        start = header.begin();
        end = header.end();

        boost::regex_search(start, end, res, expr);
        exceptionType = res[0];
        boost::replace_first(exceptionType, ":when_", "");
    }
    else
    	throw ParseError("incorrect exception handler header");


    Exception::Type type = Exception::strToExceptionType(exceptionType);

    if(type == Exception::NoType)
    	throw ParseError("incorrect exception handler name `" + exceptionType + "`");

    if(Program::getInstance().getFunction(funcName).exceptionHandlerIsExists(type))
    	throw ParseError("in function `" + funcName + "` handler on exception `" + exceptionType + "` already exists");


    CodeBlockTranslator codeTransl;
	codeTransl.setDataKeeperPtr(Program::getInstance().getFunction(funcName).getDataKeeperPtr());

	bool fEnd;

	std::string str;

	while(fEnd == false && readString(str) == false)
	{
		boost::trim(str);
		if(boost::istarts_with(str, ".end") || boost::istarts_with(str, ":when_"))
			fEnd = true;

		if(Translator::isEmptyOrComment(str) == false && fEnd == false)
			codeTransl.translate(str, m_lineNumb);
	}

	Program::getInstance().getFunction(funcName).setExceptionHandler(type, codeTransl.getCommands());

	return str;
}
