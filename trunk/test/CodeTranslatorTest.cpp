#include <stdexcept>
#include <ctime>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include "../src/CodeTranslator.h"
#include "../src/ParseError.h"


/**
@file CodeTranslatorTest.cpp
@brief Набор модульных тестов для класса CodeTranslator
*/


BOOST_AUTO_TEST_SUITE(CODETRANSLATOR_TEST_SUITE);


/**
Тест функций setDataKeeper, getDataKeeper.
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_true)
{
	DataKeeper keeper;
	keeper.addVar(Value(), "name");

	CodeTranslator translator;
	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_EQUAL(keeper.isExists("name"), true);
	BOOST_CHECK_EQUAL(keeper.isVar("name"), true);	
	BOOST_CHECK_EQUAL(keeper.isArray("name"), false);	
}



/**
Тест правильности получения кода операции.
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_getOperation)
{
	CodeTranslator translator;

	translator.translate("mov s, 1");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::MOV);

	translator.translate("add s[31],  ( const uint ) a[1]");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::ADD);

	translator.translate("sub   a,15");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::SUB);

	translator.translate("mul s, m");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::MUL);

	translator.translate("div  s   [    32    ]   ,  -1");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::DIV);

	translator.translate("jmp    lbj");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::JMP);

	translator.translate("je   _lb");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::JE);

	translator.translate("jne    _l");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::JNE);

	translator.translate("l: jg label");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::JG);

	translator.translate("l1:       l3:   cmp 1 , 2");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::CMP);

	translator.translate("l1:       nop");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::NOP);

	translator.translate("l1:ret");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::RET);

	translator.translate("l1:l2:l3:call s");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::CALL);
}



/**
Тест на корректную обработку ошибок в командах
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_operationError)
{
	CodeTranslator translator;
	BOOST_CHECK_THROW(translator.translate("mov r1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("add 1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("sub (uchar)s, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mul ( uchar) s [ 32  ], 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("unknow_command"), ParseError);
	BOOST_CHECK_THROW(translator.translate("cmp 1, 2, 3"), ParseError);
	BOOST_CHECK_THROW(translator.translate("cmp1, 2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("cmp, 1, 2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("jmp l:"), ParseError);
	BOOST_CHECK_THROW(translator.translate("jmp l, r"), ParseError);
	BOOST_CHECK_THROW(translator.translate("ret r"), ParseError);
	BOOST_CHECK_THROW(translator.translate("nop 1"), ParseError);
}





/**
Тест на корректную работу с метками при верном входе
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_labelTrue)
{
	std::list<std::string> lst;

	CodeTranslator translator;
	std::list<std::string> lst1;
	lst1.push_back("l1");

	translator.translate("l1: nop");
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());


	translator.translate("l1:");
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());


	lst1.push_back("l2");
	translator.translate("    l1:   l2: nop");
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());

	lst1.push_back("l3");
	translator.translate("l1:       l2:l3:         nop ");
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());

	translator.translate("l1:      l2:l3:         ");
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());

	translator.translate("      l1:l2:    l3:");
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());

	translator.translate("nop");
	lst1.clear();
	lst = translator.getLabelsList();
	BOOST_CHECK_EQUAL_COLLECTIONS(lst1.begin(), lst1.end(), lst.begin(), lst.end());
}




/**
Тест на корректную работу при ошибках в метках
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_labelFalse)
{
	CodeTranslator translator;
	BOOST_CHECK_THROW(translator.translate("l1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("l1:l2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("l1:   l2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("1l:"), ParseError);
	BOOST_CHECK_THROW(translator.translate(".l:"), ParseError);
	BOOST_CHECK_THROW(translator.translate("l::"), ParseError);
}



/**
Тест трансляции комманд-переходов и вызова модуля.
*/


BOOST_AUTO_TEST_CASE(CodeTranslatorTest_jump)
{
	CodeTranslator translator;
	boost::shared_ptr<LabelOperand>  lblop;

	translator.translate("jmp lbl");
	lblop = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::JMP);
	BOOST_CHECK_EQUAL(lblop -> getLabelName(), "lbl");


	translator.translate("   je      label_    ");
	lblop = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::JE);
	BOOST_CHECK_EQUAL(lblop -> getLabelName(), "label_");


	translator.translate("jne    l1");
	lblop = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::JNE);
	BOOST_CHECK_EQUAL(lblop -> getLabelName(), "l1");


	translator.translate("jl l");
	lblop = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::JL);
	BOOST_CHECK_EQUAL(lblop -> getLabelName(), "l");



	translator.translate("jg g");
	lblop = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::JG);
	BOOST_CHECK_EQUAL(lblop -> getLabelName(), "g");


	translator.translate(" call  MODULE ");
	lblop = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::CALL);
	BOOST_CHECK_EQUAL(lblop -> getLabelName(), "MODULE");
}



/**
Тест трансляции команд без операндов(ret, not)
*/


BOOST_AUTO_TEST_CASE(CodeTranslatorTest_zeroOperation)
{
	CodeTranslator translator;

	translator.translate("  nop   ");
	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::NOP);

	translator.translate("ret");
	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::RET);
}




/**
Тест правильности транслирования операндов без приведения типов
*/


BOOST_AUTO_TEST_CASE(CodeTranslatorTest_simpleOperandTwoOperation)
{
	CodeTranslator translator;
	boost::shared_ptr<VarOperand> op1, op2;

	DataKeeper keeper;
	keeper.addVar(Value(1, Value::MOD16, true, true), "var1");
	keeper.addVar(Value(2, Value::MOD16, true, true), "var2");

	Array array(3, Value::MOD16);
	array[0] = 1;
	array[1] = 2;
	array[2] = 3;

	keeper.addArray(array, "arr1");

	array = Array(3, Value::MOD16);
	array[0] = 256;
	array[1] = 257;
	array[2] = 258;

	keeper.addArray(array, "arr2");

	translator.setDataKeeperPtr(&keeper);

	translator.translate("mov var1, var2");

	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());


	BOOST_CHECK_EQUAL(op1 -> getValue(), 1);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 2);

	keeper.getVarValue("var1").setValue(3);
	BOOST_CHECK_EQUAL(op1 -> getValue(), 3);


	translator.translate("  mul arr1[1]   ,   arr2[2] ");

	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 2);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 258);

	keeper.getArray("arr1")[1].setValue(5);
	keeper.getArray("arr2")[2].setValue(6);

	BOOST_CHECK_EQUAL(op1 -> getValue(), 5);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 6);



	translator.translate("  mul arr1[0]   ,   var2");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 1);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 2);

	keeper.getArray("arr1")[0].setValue(0);
	keeper.getVarValue("var2").setValue(10);

	BOOST_CHECK_EQUAL(op1 -> getValue(), 0);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 10);


	translator.translate("  mul var1,arr2[0]");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 3);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 256);

	keeper.getArray("arr2")[0].setValue(10);
	keeper.getVarValue("var1").setValue(0);

	BOOST_CHECK_EQUAL(op1 -> getValue(), 0);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 10);


	translator.translate("  cmp var1, arr2[0] ");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 0);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 10);

	translator.translate("  cmp var1,var2");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 0);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 10);



	translator.translate("  cmp var1,  1");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 0);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 1);


	translator.translate("  cmp 512,  1");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> getValue(), 512);
	BOOST_CHECK_EQUAL(op2 -> getValue(), 1);



	BOOST_CHECK_THROW(translator.translate("mov var1, var3"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var1, var3[0]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var1[0], var3[0]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov arr1[1], var[3]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov arr3[1], arr1[1]"), ParseError);

}



/**
Проверка приведения типа.
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_castOperandTwoOperation)
{
	CodeTranslator translator;
	boost::shared_ptr<VarOperand> op1, op2;


	DataKeeper keeper;
	keeper.addVar(Value(1, Value::UNSIGNED_INT, true, true), "var1");
	keeper.addVar(Value(2, Value::UNSIGNED_CHAR, true, true), "var2");


	translator.translate("add var1, (uint) var2");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> isConstant(), false);
	BOOST_CHECK_EQUAL(op2 -> isConstant(), true);
	BOOST_CHECK_EQUAL(op1 -> getType(), Value::NO_TYPE);
	BOOST_CHECK_EQUAL(op2 -> getType(), Value::UNSIGNED_INT);


	translator.translate("  cmp var1,  1");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> isConstant(), false);
	BOOST_CHECK_EQUAL(op2 -> isConstant(), true);
	BOOST_CHECK_EQUAL(op1 -> getType(), Value::NO_TYPE);
	BOOST_CHECK_EQUAL(op2 -> getType(), Value::NO_TYPE);



	translator.translate("  cmp var1,  (uint)1");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> isConstant(), false);
	BOOST_CHECK_EQUAL(op2 -> isConstant(), true);
	BOOST_CHECK_EQUAL(op1 -> getType(), Value::NO_TYPE);
	BOOST_CHECK_EQUAL(op2 -> getType(), Value::UNSIGNED_INT);


	translator.translate("  cmp (ushort) 125,  (ushort)0");
	op1 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getFirstOperand());
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op1 -> isConstant(), true);
	BOOST_CHECK_EQUAL(op2 -> isConstant(), true);
	BOOST_CHECK_EQUAL(op1 -> getType(), Value::UNSIGNED_SHORT);
	BOOST_CHECK_EQUAL(op2 -> getType(), Value::UNSIGNED_SHORT);
}


BOOST_AUTO_TEST_SUITE_END();
