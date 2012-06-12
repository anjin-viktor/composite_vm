#include "Value.h"

Value::Value(): Operand()
{
	m_val.reset();
	m_type = NO_TYPE;
	m_isReadable = m_isWriteable = false;
}





Value::Value(long long val) throw(std::runtime_error): Operand()
{
	m_val = val;
	m_type = Value::minTypeForValue(val);
	m_isReadable = m_isWriteable = false;

	if(m_type == Value::NO_TYPE)
		throw(std::runtime_error("There is not a suitable type"));
}




Value::Value(long long val, bool readable, bool writeable) throw(std::runtime_error): Operand()
{
	m_val = val;
	m_type = Value::minTypeForValue(val);
	m_isReadable = readable;
	m_isWriteable = writeable;

	if(m_type == Value::NO_TYPE)
		throw(std::runtime_error("There is not a suitable type"));
}




Value::Value(long long val, ValueType type, bool readable, bool writeable): Operand()
{
	m_type = type;
	m_isReadable = readable;
	m_isWriteable = writeable;
	m_val = Value::longlongToType(val, type);
}




Value::~Value()
{
}




bool Value::isReadable() const
{
	return m_isReadable;
}




bool Value::isWriteable() const
{
	return m_isWriteable;
}




void Value::setReadable(bool readable)
{
	m_isReadable = readable;
}




void Value::setWriteable(bool writeable)
{
	m_isWriteable = writeable;
}




void Value::setType(ValueType type)
{
	if(m_val)
		m_val = Value::longlongToType(*m_val, type);
	m_type = type;
}




Value::ValueType Value::getType() const
{
	return m_type;
}




long long Value::getValue(ValueType type) const throw(std::runtime_error)
{
	if(m_val)
	{
		if(type = Value::NO_TYPE)
			return *m_val;
		else
			return Value::longlongToType(*m_val, type);
	}
	else
		throw std::runtime_error("Value::getValue: value does not initialize");
}





void Value::setValue(long long val) throw(std::runtime_error)
{

	if(m_type == Value::NO_TYPE)
		m_type = minTypeForValue(val);

	m_val = longlongToType(val, m_type);
}




Value::ValueType Value::minTypeForValue(long long val)
{
	if(val < 0)
	{
		unsigned long long tmp = std::abs(val);
		if(tmp <= (1 << 7))
			return SIGNED_CHAR;
		else if(tmp <= (1 << 15))
			return SIGNED_SHORT;
		else if(tmp <= (static_cast<long long>(1) << 31))
			return SIGNED_INT;
		else
			return NO_TYPE;
	}
	else
	{
		if(val < (1 << 8))
			return UNSIGNED_CHAR;
		else if(val < (1 << 16))
			return UNSIGNED_SHORT;
		else if(val < (static_cast<long long>(1) << 32))
			return UNSIGNED_INT;
		else
			return NO_TYPE;
	}
}




long long Value::longlongToType(long long val, ValueType type) throw(std::runtime_error)
{

	switch(type)
	{
		case Value::SIGNED_CHAR:
			return static_cast<signed char>(val);

		case Value::UNSIGNED_CHAR:
			return static_cast<unsigned char>(val);

		case Value::MOD8:
			return static_cast<unsigned char>(val);

		case Value::SIGNED_SHORT:
			return static_cast<signed short>(val);

		case Value::UNSIGNED_SHORT:
			return static_cast<unsigned short>(val);

		case Value::MOD16:
			return static_cast<unsigned short>(val);

		case Value::SIGNED_INT:
			return static_cast<signed int>(val);

		case Value::UNSIGNED_INT:
			return static_cast<unsigned int>(val);

		case Value::MOD32:
			return static_cast<unsigned int>(val);

		case Value::NO_TYPE:
			throw std::runtime_error("Value::longlongToType: don`t can fetch valut to type NO_TYPE");
	};
}
