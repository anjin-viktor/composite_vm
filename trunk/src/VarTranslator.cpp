#include "VarTranslator.h"


VarTranslator::VarTranslator()
{
	m_grammar.setNamePtr(&m_valName);
	m_grammar.setValuePtr(&m_val);
}




std::pair<std::string, Value> VarTranslator::translate(std::string str)
{
	m_grammar.clearValue();
    str_t_it begin = str.begin(), end = str.end();

    bool success = qi::parse(begin, end, m_grammar);

     if(success && begin == end)
		return std::pair<std::string, Value>(m_valName, m_val);
	else
	{
		std::cerr << "error in parse\n" << std::string(begin, end);
	}
}
/*




void setValue(Value *pval, long long val)
{
	std::cerr << "val: " << val << '\n';
	std::cerr << (int) pval;

	pval -> setValue(val);
	pval -> setReadable(true);
}




void setNoWriteable(Value *pval)
{
	pval -> setWriteable(false);

}




void addCharToVarName(std::string *pname, char ch)
{
	std::cerr << "addCharToVarName: " << ch << '\n';
	*pname += ch;
}




void setValType(Value *pval, std::string type)
{
	std::cerr << "type: " << type << '\n';
	std::cerr << "1\n";
	Value::strToValueType(type);
	std::cerr << "2\n";
	std::cerr << (int) pval;
//	pval -> setReadable(false);
	std::cerr << "3\n";
//	pval -> setType(Value::strToValueType(type));
	std::cerr << "3\n";
}
*/
