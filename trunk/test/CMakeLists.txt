set (PROJECT 
	COMPOZIT_VM_TESTS)
 
project (${PROJECT})

 
set (SOURCES
	../src/Value.cpp
	../src/LabelOperand.cpp
	../src/VarOperand.cpp
	../src/Command.cpp
	../src/VarTranslator.cpp
	../src/ParseError.cpp
	../src/Array.cpp
	../src/DataKeeper.cpp
	../src/Label.cpp
	../src/CodeTranslator.cpp
	../src/ArrayOperand.cpp
	../src/CodeBlockTranslator.cpp
	../src/Function.cpp
	../src/Translator.cpp
	../src/HeaderTranslator.cpp
	../src/Program.cpp
	../src/CallOperand.cpp

	ValueTest.cpp
	LabelOperandTest.cpp
	VarOperandTest.cpp
	CommandTest.cpp
	VarTranslatorTest.cpp
	ParseErrorTest.cpp
	ArrayTest.cpp
	DataKeeperTest.cpp
	LabelTest.cpp
	CodeTranslatorTest.cpp
	ArrayOperandTest.cpp
	CodeBlockTranslatorTest.cpp
	FunctionTest.cpp
	TranslatorTest.cpp
	HeaderTranslatorTest.cpp
	ProgramTest.cpp
	CallOperandTest.cpp
	)
 
set (BOOST_COMPONENTS 
		unit_test_framework
		regex
	)

set (Boost_USE_STATIC_LIBS ON)


include_directories(../src/)
include_directories(${Boost_INCLUDE_DIRS})


find_package (Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)


add_executable (${PROJECT} ${SOURCES})

target_link_libraries (${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

enable_testing()
add_test(${PROJECT} ${PROJECT})
