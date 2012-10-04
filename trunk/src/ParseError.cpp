#include "ParseError.h"

ParseError::ParseError(const std::string &str)
{
	std::stringstream ss;
	ss << str << " at file " <<  TranslatedFileInfo::getInstance().getFile() << ": " <<
				TranslatedFileInfo::getInstance().getLineNo();

	m_msg = ss.str();
}




ParseError::~ParseError() throw()
{
}




const char *ParseError::what() const throw()
{
	return m_msg.c_str();
}




void ParseError::setMsg(const std::string &str)
{
	m_msg = str;
}
