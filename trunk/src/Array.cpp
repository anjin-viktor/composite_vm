#include "Array.h"

Array::Array()
{
	m_isWriteable = true;
	m_type = Value::NO_TYPE;
}




Array::Array(std::size_t size)
{
	if(size)
		m_parr = boost::shared_ptr<std::vector<Value> >(new std::vector<Value>(size));
	m_isWriteable = true;
	m_type = Value::NO_TYPE;
}




Array::Array(std::size_t size, Value::ValueType type)
{
	if(size)
		m_parr = boost::shared_ptr<std::vector<Value> >(new std::vector<Value>(size));
	m_isWriteable = true;

	for(int i=0; i<size; i++)
		m_parr -> at(i).setType(type);

	m_type = type;
}




Array::~Array()
{
}




void Array::setType(Value::ValueType type)
{
	if(m_parr)
		for(int i=0; i<m_parr -> size(); i++)
			m_parr -> at(i).setType(type);	

	m_type = type;
}




void Array::resize(std::size_t size)
{
	int start = 0;
	if(m_parr)
		start = m_parr -> size();

	if(m_parr == NULL)
		m_parr = boost::shared_ptr<std::vector<Value> >(new std::vector<Value>(size));
	else if(size > 0)
		m_parr -> resize(size);
	else
		m_parr.reset();

	for(int i=start; i<size; i++)
		m_parr -> at(i).setType(m_type);
}




void Array::setWriteable(bool writeable)
{
	m_isWriteable = writeable;
}




void Array::setReadableAll(bool readable)
{
	if(m_parr)
		for(int i=0; i<m_parr->size(); i++)
			m_parr -> at(i).setReadable(readable);
}




void Array::setWriteableAll(bool writeable)
{
	if(m_parr)
		for(int i=0; i<m_parr->size(); i++)
			m_parr -> at(i).setWriteable(writeable);
}




std::size_t Array::size() const
{
	if(m_parr)
		return m_parr -> size();
	else
		return 0;
}




bool Array::isWriteable() const
{
	return m_isWriteable;
}




const Value &Array::operator[] (std::size_t n) const
{
	if(m_parr == NULL || n >= m_parr -> size())
		throw std::runtime_error("out of range");

	return m_parr -> at(n);
}




Value &Array::operator[] (std::size_t n)
{
	if(m_parr == NULL || n >= m_parr -> size())
		throw std::runtime_error("out of range");


	return m_parr -> at(n);
}
