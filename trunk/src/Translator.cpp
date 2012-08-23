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

	m_in.close();
}





void Translator::translateFunction(const std::string &header)
{
	std::string str;
	std::string funcName = HeaderTranslator::getNameFromStr(header);

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
		throw ParseError("unexpected end");

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
			throw ParseError("unexpected end");
		else
			throw ParseError(str + " not expected");
	}

	fEnd = false;

	Program::getInstance().addFunction(func);
	Program::getInstance().getFunction(funcName).setDataKeeper(varTransl.getDataKeeper());

	HeaderTranslator headerTransl;
	headerTransl.setDataKeeperPtr(Program::getInstance().getFunction(funcName).getDataKeeperPtr());
	headerTransl.translate(header);

	CodeBlockTranslator codeTransl;
	codeTransl.setDataKeeperPtr(Program::getInstance().getFunction(funcName).getDataKeeperPtr());

	while(readString(str) == false)
	{
		boost::trim(str);
		if(boost::istarts_with(str, ".end") || boost::istarts_with(str, ".exception"))
			fEnd = true;

		if(Translator::isEmptyOrComment(str) == false && fEnd == false)
			codeTransl.translate(str, m_lineNumb);
	}

	Program::getInstance().getFunction(funcName).setCommands(codeTransl.getCommands());

}
