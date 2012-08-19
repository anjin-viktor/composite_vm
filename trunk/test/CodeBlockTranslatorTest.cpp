#include <vector>

#include <boost/test/unit_test.hpp>

#include "../src/CodeBlockTranslator.h"
#include "../src/DataKeeper.h"
#include "Command.h"
#include "LabelOperand.h"
#include "ParseError.h"



/**
@file CodeBlockTranslatorTest.cpp
@brief Набор модульных тестов для класса CodeBlockTranslator
*/


BOOST_AUTO_TEST_SUITE(CODE_BLOCK_TRANSLATOR_TEST_SUITE);


/**
Тест конструкторов;
*/

BOOST_AUTO_TEST_CASE(CodeBlockTranslator_constr)
{
	DataKeeper keeper;
	CodeBlockTranslator transl1, transl2(&keeper);
}


/**
Тест правильности трансляции на верных входных данных
*/

BOOST_AUTO_TEST_CASE(CodeBlockTranslator_true_1)
{
	DataKeeper keeper;
	keeper.addVar(Value(2, Value::MOD16, true, true), "v1");
	keeper.addVar(Value(4, Value::MOD16, true, true), "v2");
	keeper.addVar(Value(8, Value::MOD16, true, true), "v3");


	CodeBlockTranslator translator;
	translator.setDataKeeperPtr(&keeper);


	translator.translate("    l1: ");
	translator.translate("    l2: ");
	translator.translate("nop");
	translator.translate("mov v1, 1");
	translator.translate("mov v1, v2");
	translator.translate("mov v2, 1");
	translator.translate("mov v3, 1");
	translator.translate("jmp l2");
	translator.translate("je l3");
	translator.translate("add v1, v2");
	translator.translate("l3:");
	translator.translate("jmp l1");
	translator.translate("nop");


	std::vector<Command> v = translator.getCommands();

	BOOST_CHECK_EQUAL(v[0].getOperationType(), Command::NOP);
	BOOST_CHECK_EQUAL(v[1].getOperationType(), Command::MOV);
	BOOST_CHECK_EQUAL(v[2].getOperationType(), Command::MOV);
	BOOST_CHECK_EQUAL(v[3].getOperationType(), Command::MOV);
	BOOST_CHECK_EQUAL(v[4].getOperationType(), Command::MOV);
	BOOST_CHECK_EQUAL(v[5].getOperationType(), Command::JMP);
	BOOST_CHECK_EQUAL(v[6].getOperationType(), Command::JE);
	BOOST_CHECK_EQUAL(v[7].getOperationType(), Command::ADD);
	BOOST_CHECK_EQUAL(v[8].getOperationType(), Command::JMP);
	BOOST_CHECK_EQUAL(v[9].getOperationType(), Command::NOP);


	boost::shared_ptr<LabelOperand> lblOp;
	boost::shared_ptr<VarOperand> varOp1, varOp2;


	varOp1 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[1].getFirstOperand());
	varOp2 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[1].getSecondOperand());
	BOOST_CHECK_EQUAL(varOp1 -> getValue(), 2);
	BOOST_CHECK_EQUAL(varOp2 -> getValue(), 1);

	varOp1 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[2].getFirstOperand());
	varOp2 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[2].getSecondOperand());
	BOOST_CHECK_EQUAL(varOp1 -> getValue(), 2);
	BOOST_CHECK_EQUAL(varOp2 -> getValue(), 4);

	varOp1 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[3].getFirstOperand());
	varOp2 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[3].getSecondOperand());
	BOOST_CHECK_EQUAL(varOp1 -> getValue(), 4);
	BOOST_CHECK_EQUAL(varOp2 -> getValue(), 1);

	varOp1 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[4].getFirstOperand());
	varOp2 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[4].getSecondOperand());
	BOOST_CHECK_EQUAL(varOp1 -> getValue(), 8);
	BOOST_CHECK_EQUAL(varOp2 -> getValue(), 1);

	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[5].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "l2");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 0);


	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[6].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "l3");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 8);

	varOp1 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[7].getFirstOperand());
	varOp2 = boost::dynamic_pointer_cast<VarOperand, Operand>(v[7].getSecondOperand());
	BOOST_CHECK_EQUAL(varOp1 -> getValue(), 2);
	BOOST_CHECK_EQUAL(varOp2 -> getValue(), 4);

	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[8].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "l1");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 0);
}



/**
Проверка корректности работы с метками при верном коде. 
*/
BOOST_AUTO_TEST_CASE(CodeBlockTranslator_lblTrue)
{
	DataKeeper keeper;
	CodeBlockTranslator translator(&keeper);

	translator.translate("l1:   L2: ");
	translator.translate("jmp l1");
	translator.translate("je L2");
	translator.translate("jg l3");
	translator.translate("jne l4");
	translator.translate(" l3:l4:");

	std::vector<Command> v = translator.getCommands();

	BOOST_CHECK_EQUAL(v[0].getOperationType(), Command::JMP);
	BOOST_CHECK_EQUAL(v[1].getOperationType(), Command::JE);
	BOOST_CHECK_EQUAL(v[2].getOperationType(), Command::JG);
	BOOST_CHECK_EQUAL(v[3].getOperationType(), Command::JNE);

	boost::shared_ptr<LabelOperand> lblOp;

	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[0].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "l1");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 0);

	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[1].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "L2");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 0);

	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[2].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "l3");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 4);

	lblOp = boost::dynamic_pointer_cast<LabelOperand, Operand> (v[3].getFirstOperand());
	BOOST_CHECK_EQUAL(lblOp -> getLabelName(), "l4");
	BOOST_CHECK_EQUAL(lblOp -> getCommandOffset(), 4);
}


/**
Проверка корректности работы с метками при неверном коде.
*/
BOOST_AUTO_TEST_CASE(CodeBlockTranslator_lblFalse)
{
	DataKeeper keeper;
	CodeBlockTranslator translator(&keeper);

	translator.translate("l1:");
	translator.translate("l2:");

	translator.translate("jmp l1");
	translator.translate("je l2");
	translator.translate("jmp l3");


	BOOST_CHECK_THROW(translator.getCommands(), ParseError);
}


BOOST_AUTO_TEST_SUITE_END();
