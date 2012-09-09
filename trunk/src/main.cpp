#include <iostream>
#include <stdexcept>

#include "Translator.h"
#include "CodeExecuter.h"
#include "ParseError.h"

int main(int argc, char **argv)
{
	if(argc != 2)
		std::cerr << "Usage: " << argv[0] << "file_name" << std::endl;


	Translator translator(argv[1]);

	try
	{
		translator.translate();
		CodeExecuter exec;
		exec.exec();
	}
	catch(std::runtime_error err)
	{
		std::cerr << "Error: " << err.what() << std::endl;
		return 1;
	}
	catch(ParseError err)
	{
		std::cerr << "Error: " << err.what() << std::endl;
		return 1;
	}

	return 0;
}
