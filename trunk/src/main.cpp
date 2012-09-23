#include <iostream>
#include <stdexcept>
#include <fstream>

#include "Translator.h"
#include "CodeExecuter.h"
#include "ParseError.h"

int main(int argc, char **argv)
{
	if(argc < 2 || argc > 3)
	{
		std::cerr << "Usage: " << argv[0] << " code_file [out_file]" << std::endl;
		return 1;
	}

	Translator translator(argv[1]);

	std::ofstream out;

	if(argc == 3)
	{
		out.open(argv[2], std::ios_base::out);
		if(!out)
		{
			std::cerr << "Error in opening file `" << argv[2] << "`" << std::endl;
			return 2;
		}
	}


	try
	{
		translator.translate();
			
		CodeExecuter exec;

		if(out.is_open())
			exec.setOutputStream(out);

		exec.exec();
	}
	catch(std::runtime_error err)
	{
		std::cout << "Error: " << err.what() << std::endl;
		return 1;
	}
	catch(ParseError err)
	{
		std::cout << "Error: " << err.what() << std::endl;
		return 1;
	}
/*	catch(std::constraint_error err)
	{
		std::cout << "Error: " << err.what() << std::endl;
		return 1;		
	}
*/
	if(out.is_open())
		out.close();

	return 0;
}
