#include "Exception.h"

Exception::Type Exception::strToExceptionType(const std::string &str)
{
	using namespace Exception;

	if(str == "numeric_error")
		return NumericError;
	else if(str == "constraint_error")
		return ConstraintError;
	else
		return NoType;
}




std::string Exception::exceptionTypeToStr(Exception::Type except)
{
	using namespace Exception;

	switch(except)
	{
		case NumericError:
			return "numeric_error";
		case ConstraintError:
			return "constraint_error";
		case NoType:
			return "no_type";
	};
}
