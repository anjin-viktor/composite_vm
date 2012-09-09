set (PROJECT 
	composite_vm)
 
project (${PROJECT})

 
set (SOURCES
		Value.cpp
		LabelOperand.cpp
		VarOperand.cpp
		Command.cpp
		VarTranslator.cpp
		ParseError.cpp
		Array.cpp
		Label.cpp
		CodeTranslator.cpp
		ArrayOperand.cpp
		CodeBlockTranslator.cpp
		Translator.cpp
		HeaderTranslator.cpp
		Program.cpp
		CallOperand.cpp
		Exception.cpp
		Context.cpp
		CodeExecuter.cpp
		DataKeeper.cpp
		Function.cpp
		main.cpp
	)

set (BOOST_COMPONENTS 
		unit_test_framework
		regex
	)

set (Boost_USE_STATIC_LIBS ON)


include_directories(${Boost_INCLUDE_DIRS})


find_package (Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)


add_executable (${PROJECT} ${SOURCES})

target_link_libraries (${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
