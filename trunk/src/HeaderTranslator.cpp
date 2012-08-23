#include "HeaderTranslator.h"


HeaderTranslator::HeaderTranslator()
{
	m_pdata = NULL;
}



HeaderTranslator::~HeaderTranslator()
{
}




void HeaderTranslator::setDataKeeperPtr(DataKeeper *pdata)
{
	m_pdata = pdata;
	m_grammar.setDataKeeperPtr(pdata);
}




std::string HeaderTranslator::getName() const
{
	return m_grammar.getName();
}




void HeaderTranslator::translate(std::string str)
{
    std::string::iterator begin = str.begin(), end = str.end();

    m_grammar.clear();

    if(m_pdata)
    	m_pdata -> clear();

    bool success = qi::parse(begin, end, m_grammar);

    if(!success || begin != end)
		throw ParseError("stopped at: " + std::string(begin, end));
}





std::string HeaderTranslator::getNameFromStr(const std::string &str)
{
    boost::regex expr("^\\s*\\.name\\s*[\\l\\u_]\\w*");
    boost::regex cmpExpr("^\\s*\\.name\\s*[\\l\\u_]\\w*(\\s.*)*");
  
    boost::smatch res;
    std::string::const_iterator start = str.begin();
    std::string::const_iterator end = str.end();

    if(boost::regex_match(start, end, res, cmpExpr))
    {
        start = str.begin();
        end = str.end();

        boost::regex_search(start, end, res, expr);
        std::string s = res[0];
        boost::trim(s);
        boost::replace_first(s, ".name", "");
        boost::trim(s);

        return s;
    }
    else
        throw std::runtime_error("HeaderTranslator::getNameFromStr: incorrect parameter");

    return "";
}