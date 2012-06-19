set (PROJECT 
	COMPOZIT_VM_TESTS)
 
project (${PROJECT})

 
set (SOURCES
	../src/Value.cpp
	ValueTest.cpp
	../src/LabelOperand.cpp
	LabelOperandTest.cpp
	../src/VarOperand.cpp
	VarOperandTest.cpp
	../src/Command.cpp
	CommandTest.cpp
	VarTranslatorTest.cpp
	../src/VarTranslator.cpp
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
