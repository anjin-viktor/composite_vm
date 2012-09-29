cmake_minimum_required(VERSION 2.8)

set (PROJECT 
	composite_vm)

set (TEST_DIR ../test/)
set (REGRESSION_DIR ../regressions/)

set (PROJECT_LIB lib_${PROJECT})

project (${PROJECT})



set (LIB_SOURCES
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
	)

set (SOURCES main.cpp)

#set (TESTS_SOURCES
#		../test/ValueTest.cpp
#		../test/LabelOperandTest.cpp
#		../test/VarOperandTest.cpp
#		../test/CommandTest.cpp
#		../test/VarTranslatorTest.cpp
#		../test/ParseErrorTest.cpp
#		../test/ArrayTest.cpp
#		../test/DataKeeperTest.cpp
#		../test/LabelTest.cpp
#		../test/CodeTranslatorTest.cpp
#		../test/ArrayOperandTest.cpp
#		../test/CodeBlockTranslatorTest.cpp
#		../test/FunctionTest.cpp
#		../test/TranslatorTest.cpp
#		../test/HeaderTranslatorTest.cpp
#		../test/ProgramTest.cpp
#		../test/CallOperandTest.cpp
#		../test/ExceptionTest.cpp
#		../test/ContextTest.cpp
#		../test/CodeExecuterTest.cpp
#	)

set (BOOST_COMPONENTS 
		unit_test_framework
		regex
	)

set (Boost_USE_STATIC_LIBS ON)

add_library (${PROJECT_LIB} STATIC ${LIB_SOURCES})

include_directories(${Boost_INCLUDE_DIRS})
#include_directories(./)


find_package (Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)


#add_executable (${TEST} ${TESTS_SOURCES})
#target_link_libraries (${TEST} ${PROJECT_LIB} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

add_executable (${PROJECT} ${SOURCES})
target_link_libraries (${PROJECT} ${PROJECT_LIB} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

enable_testing()
#add_test(${TEST} ${TEST})

add_subdirectory(${TEST_DIR} ${TEST_DIR})
add_subdirectory(${REGRESSION_DIR} ${REGRESSION_DIR})

