#include "TranslatedFileInfo.h"

TranslatedFileInfo TranslatedFileInfo::m_instance;

TranslatedFileInfo::TranslatedFileInfo()
{
	m_lineNo = 0;
}


TranslatedFileInfo::~TranslatedFileInfo()
{
}


TranslatedFileInfo &TranslatedFileInfo::getInstance()
{
	return m_instance;
}



std::string TranslatedFileInfo::getFile() const
{
	return m_fileName;
}




std::size_t TranslatedFileInfo::getLineNo() const
{
	return m_lineNo;
}




void TranslatedFileInfo::setFile(const std::string &name)
{
	m_fileName = name;
}




void TranslatedFileInfo::clearLineNo()
{
	m_lineNo = 0;
}




void TranslatedFileInfo::incLineNo()
{
	m_lineNo++;
}
