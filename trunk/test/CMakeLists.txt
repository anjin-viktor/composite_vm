set (TEST
	COMPOZIT_VM_UNIT_TESTS)
 
project (${TEST})

 
set (SOURCES

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
	ExceptionTest.cpp
	ContextTest.cpp
	CodeExecuterTest.cpp
	)
 
set (BOOST_COMPONENTS 
		unit_test_framework
		regex
	)

set (Boost_USE_STATIC_LIBS ON)


include_directories(../src/)
include_directories(${Boost_INCLUDE_DIRS})


find_package (Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)


add_executable (${TEST} ${SOURCES})

target_link_libraries (${TEST} ${PROJECT_LIB} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} )

add_test(${TEST} ${TEST})
