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
	char s[2048];

	if(m_in.is_open() == false)
		m_in.open(m_inputFileName.c_str(), std::ifstream::in);

	std::stringbuf sb;

	m_in.get(sb, '\n');

	str = sb.str();

	return m_in.eof();
}



bool Translator::isEmptyOrComment(const std::string &str)
{
	return str.length() == 0 || boost::istarts_with(str, ";");
}



void Translator::translate()
{
	std::string str;

	while(readString(str) == false)
	{
		boost::trim(str);
		if(Translator::isEmptyOrComment(str) == false)
		{
			if(boost::istarts_with(str, ".name") == false)
				throw ParseError(str + " not expected");
			
		}
	}
}

