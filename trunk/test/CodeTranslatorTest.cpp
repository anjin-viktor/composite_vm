#include <stdexcept>
#include <ctime>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include "../src/CodeTranslator.h"
#include "../src/ParseError.h"
#include "../src/LabelOperand.h"
#include "../src/CallOperand.h"


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


	translator.translate("l1:   rsz arr,1");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::RSZ);

	translator.translate("rsz arr    ,    ch");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::RSZ);

	translator.translate("aout str");
	BOOST_CHECK_EQUAL(translator.getOperation(), Command::AOUT);
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
	BOOST_CHECK_THROW(translator.translate("rsz arr, 1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("aout"), ParseError);
	BOOST_CHECK_THROW(translator.translate("rsz arr"), ParseError);
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
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(2, Value::MOD16, true, true);
	array[2] = Value(3, Value::MOD16, true, true);

	keeper.addArray(array, "arr1");

	array = Array(3, Value::MOD16);
	array[0].setValue(256);
	array[1].setValue(257);
	array[2].setValue(258);
	array[2].setReadable(true);
	array[1].setReadable(true);
	array[0].setReadable(true);

	keeper.addArray(array, "arr2");

	Array arr(3, Value::MOD8);
	arr[0].setValue(0);
	arr[1].setValue(1);
	arr[2].setValue(2);
	arr[2].setReadable(true);
	arr[1].setReadable(true);
	arr[0].setReadable(true);

	keeper.addArray(arr, "arr");

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



	translator.translate("rsz arr2, 5");
	BOOST_CHECK_EQUAL(dynamic_cast<ArrayOperand*>(translator.getCommand().getFirstOperand().get()) -> getArrayPtr(), &(keeper.getArray("arr2")));




	translator.translate("rsz  arr, 15");
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());
	BOOST_CHECK_EQUAL(dynamic_cast<ArrayOperand*>(translator.getCommand().getFirstOperand().get()) -> getArrayPtr(), &(keeper.getArray("arr")));
	BOOST_CHECK_EQUAL(op2 -> getValue(), 15);


	translator.translate("rsz  arr, var1");
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());
	BOOST_CHECK_EQUAL(dynamic_cast<ArrayOperand*>(translator.getCommand().getFirstOperand().get()) -> getArrayPtr(), &(keeper.getArray("arr")));
	BOOST_CHECK_EQUAL(op2 -> getValue(), 0);



	BOOST_CHECK_THROW(translator.translate("mov not_exists_array_name"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var1, var3"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var1, var3[0]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var1[0], var3[0]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov arr1[1], var[3]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov arr3[1], arr1[1]"), ParseError);
}



/**
Проверка на верную работу при несоответствующем классе операндов
*/


BOOST_AUTO_TEST_CASE(CodeTranslatorTest_opClassError)
{
	CodeTranslator translator;
	boost::shared_ptr<VarOperand> op1, op2;

	DataKeeper keeper;
	keeper.addVar(Value(1, Value::MOD16, true, true), "var1");
	keeper.addVar(Value(2, Value::MOD8, true, true), "var2");

	Array array(1, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);

	keeper.addArray(array, "arr1");

	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_THROW(translator.translate("mov arr1, var1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("cmp arr1, var1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("sub var1, arr1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("div arr1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("div 1, var1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("div 1, arr1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("aout arr1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("rsz var1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("rsz 1, var2"), ParseError);
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
	BOOST_CHECK_EQUAL(op2 -> getAfterCastType(), Value::UNSIGNED_INT);


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



	translator.translate("rsz arr, (sshort) 155");
	op2 = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getSecondOperand());

	BOOST_CHECK_EQUAL(op2 -> isConstant(), true);
	BOOST_CHECK_EQUAL(op2 -> getType(), Value::SIGNED_SHORT);

}



/**
Тест генерирования иселючений вслучае нарушения прав доступа.
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_accessPermitionsFalse)
{
	CodeTranslator translator;

	DataKeeper keeper;
	keeper.addVar(Value(1, Value::MOD16, false, true), "var1");
	keeper.addVar(Value(2, Value::MOD16, true, false), "var2");
	keeper.addVar(Value(2, Value::MOD16, true, true), "var3");


	Array array(3, Value::MOD16);
	array[0] = 1;
	array[1] = 2;
	array[2] = 3;

	keeper.addArray(array, "arr1");

	array = Array(3, Value::MOD16);
	array[0] = 256;
	array[1] = 257;
	array[1].setWriteable(false);
	array[2] = Value();
	keeper.addArray(array, "arr2");

	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_THROW(translator.translate("add var2, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var1, var1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var2, var1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov var3, var1"), ParseError);
	BOOST_CHECK_NO_THROW(translator.translate("mov var1, var2"));

	BOOST_CHECK_THROW(translator.translate("mov arr2[1], 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("mov arr2[0], arr2[2]"), ParseError);


	keeper.getArray("arr1").setWriteable(false);
	BOOST_CHECK_THROW(translator.translate("rsz arr1, 5"), ParseError);
	keeper.getArray("arr1").setWriteable(true);
	keeper.getArray("arr1")[0].setWriteable(true);
	keeper.getArray("arr1")[1].setWriteable(true);
	keeper.getArray("arr1")[2].setWriteable(true);
	BOOST_CHECK_NO_THROW(translator.translate("rsz arr1, 5"));

	array = Array(2, Value::MOD8);
	array[0].setReadable(true);
	array[1].setReadable(true);
	keeper.addArray(array, "str");

	BOOST_CHECK_NO_THROW(translator.translate("aout str"));
	keeper.getArray("str")[1].setReadable(true);
	BOOST_CHECK_NO_THROW(translator.translate("aout str"));


}



BOOST_AUTO_TEST_CASE(CodeTranslatorTest_aout)
{
	CodeTranslator translator;
	DataKeeper keeper;
	translator.setDataKeeperPtr(&keeper);
	keeper.addVar(Value(1, Value::MOD16, false, true), "var1");
	keeper.addVar(Value(2, Value::UNSIGNED_INT, true, false), "var2");


	Array array(3, Value::MOD8);
	array[0].setValue(256);
	array[1].setValue(257);
	array[2].setValue(258);
	array[2].setReadable(true);
	array[1].setReadable(true);
	array[0].setReadable(true);
	array[0].setWriteable(true);
	array[1].setWriteable(true);
	array[2].setWriteable(true);

	keeper.addArray(array, "str");

	Array arr(3, Value::MOD16);
	arr[0].setValue(256);
	arr[1].setValue(257);
	arr[2].setValue(258);
	arr[2].setReadable(true);
	arr[1].setReadable(true);
	arr[0].setReadable(true);
	keeper.addArray(arr, "arr");

	translator.translate("aout str");
	BOOST_CHECK_EQUAL(dynamic_cast<ArrayOperand*>(translator.getCommand().getFirstOperand().get()) -> getArrayPtr(), &(keeper.getArray("str")));
	BOOST_CHECK_THROW(translator.translate("aout arr"), ParseError);

	array[1].setReadable(false);
	BOOST_CHECK_NO_THROW(translator.translate("aout str"));

	translator.translate("rsz arr, 5");
	translator.translate("rsz arr, var2");
	BOOST_CHECK_THROW(translator.translate("rsa arr, var1"), ParseError);
	arr.setWriteable(false);
	BOOST_CHECK_THROW(translator.translate("rsa arr, 5"), ParseError);
}




BOOST_AUTO_TEST_CASE(CodeTranslatorTest_comment)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate(";comment"));
	BOOST_CHECK_NO_THROW(translator.translate("mul var, 1;comment"));
	BOOST_CHECK_NO_THROW(translator.translate("jmp lbl 					;comment"));
}



/**
Тест для команды call
*/
BOOST_AUTO_TEST_CASE(CodeTranslatorTest_call)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate("call label"));
	BOOST_CHECK_NO_THROW(translator.translate("call label, l"));
	BOOST_CHECK_NO_THROW(translator.translate("    call    label  ,   v1,(uchar)v2 "));
	BOOST_CHECK_NO_THROW(translator.translate("    call    label  ,   v1,v2,     123 "));

	BOOST_CHECK_THROW(translator.translate("call 123"), ParseError);
	BOOST_CHECK_THROW(translator.translate("call :label"), ParseError);


	DataKeeper keeper;
	keeper.addVar(Value(1, Value::MOD16, true, true), "var1");
	keeper.addVar(Value(1, Value::SIGNED_INT, true, false), "var2");
	keeper.addVar(Value(1, Value::UNSIGNED_INT, false, false), "var3");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(1, Value::MOD16, false, false);
	array[2];

	keeper.addArray(array, "arr");
	translator.setDataKeeperPtr(&keeper);
	BOOST_CHECK_NO_THROW(translator.translate("call    label, var1 "));
	BOOST_CHECK_NO_THROW(translator.translate("call    label, var2 "));
	BOOST_CHECK_NO_THROW(translator.translate("call    label, (uint)   var2, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("call    label_, arr"));
	BOOST_CHECK_NO_THROW(translator.translate("call    label_, arr, var2"));
	BOOST_CHECK_NO_THROW(translator.translate("call    label_, arr[1]"));
	BOOST_CHECK_NO_THROW(translator.translate("call    label_, (   mod8   ) arr[2]"));

	BOOST_CHECK_THROW(translator.translate("call    label_, arr_[2]"), ParseError);
	BOOST_CHECK_THROW(translator.translate("call    label_, var4"), ParseError);



	translator.translate("call    label_, arr, (uint)var2, 123");

	boost::shared_ptr<LabelOperand> name = boost::dynamic_pointer_cast<LabelOperand, Operand>(translator.getCommand().getFirstOperand());
	BOOST_CHECK_EQUAL(name -> getLabelName(), "label_");

	boost::shared_ptr<CallOperand> op = boost::dynamic_pointer_cast<CallOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(op -> isArray(), true);
	BOOST_CHECK_EQUAL(op -> isValue(), false);
	BOOST_CHECK_EQUAL(op -> getArray().size(), 3);
	BOOST_CHECK_EQUAL(op -> getArray()[0].getValue(), 1);
	BOOST_CHECK_EQUAL(op -> getArray()[1].getValue(), 1);

	op = boost::dynamic_pointer_cast<CallOperand, Operand>(translator.getCommand().getOperand(3));
	BOOST_CHECK_EQUAL(op -> isArray(), false);
	BOOST_CHECK_EQUAL(op -> isValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue().getValue(), 123);
	BOOST_CHECK_EQUAL(op -> getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(op -> getValue().isWriteable(), false);


	op = boost::dynamic_pointer_cast<CallOperand, Operand>(translator.getCommand().getOperand(2));
	BOOST_CHECK_EQUAL(op -> isArray(), false);
	BOOST_CHECK_EQUAL(op -> isValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue().getValue(), 1);
	BOOST_CHECK_EQUAL(op -> getValue().isReadable(), true);
	BOOST_CHECK_EQUAL(op -> getValue().isWriteable(), false);
	BOOST_CHECK_EQUAL(op -> getValueType(), Value::UNSIGNED_INT);
}




/**
Тест трансляции команды GTEL
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_gtel)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate("gtel v, arr, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("lbl: gtel tmp , str, n ; "));
	BOOST_CHECK_NO_THROW(translator.translate("lbl: gtel tmp , str   , n; "));
	BOOST_CHECK_NO_THROW(translator.translate("lbl:gtel 	tmp,str,n"));
	BOOST_CHECK_NO_THROW(translator.translate("lbl:gtel 	tmp,str,1"));
	BOOST_CHECK_NO_THROW(translator.translate("gtel v[1], arr, 1"));


	BOOST_CHECK_THROW(translator.translate("lbl:gtel 1,str,n"), ParseError);
	BOOST_CHECK_THROW(translator.translate("lbl:gtel k,1,n"), ParseError);


	DataKeeper keeper;
	keeper.addVar(Value(125, Value::MOD16, false, true), "var1");
	keeper.addVar(Value(100, Value::SIGNED_INT, true, false), "var2");
	keeper.addVar(Value(1, Value::UNSIGNED_INT, false, false), "var3");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(1, Value::MOD16, false, false);
	array[2];

	keeper.addArray(array, "arr");
	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_NO_THROW(translator.translate("gtel var1, arr, 1"));

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::GTEL);


	boost::shared_ptr<ArrayOperand> parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(parr -> getArrayPtr(), &keeper.getArray("arr"));

	boost::shared_ptr<VarOperand> op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(2));
	BOOST_CHECK_EQUAL(op -> getValue(), 1);


	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));
	BOOST_CHECK_EQUAL(op -> hasValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue(), 125);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), true);


	BOOST_CHECK_NO_THROW(translator.translate("gtel var1, arr, var2"));

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::GTEL);

	parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(parr -> getArrayPtr(), &keeper.getArray("arr"));

	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(2));
	BOOST_CHECK_EQUAL(op -> getValue(), 100);


	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));
	BOOST_CHECK_EQUAL(op -> hasValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue(), 125);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), true);


	BOOST_CHECK_THROW(translator.translate("gtel var2, arr, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("gtel var2, arr, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("gtel (mod16)var2, arr, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("gtel var1, arr, var3"), ParseError);
	BOOST_CHECK_THROW(translator.translate("gtel var1, arr, var4"), ParseError);

	keeper.getVarValue("var2").setReadable(false);
	BOOST_CHECK_THROW(translator.translate("gtel var1, arr, var2"), ParseError);
	keeper.getVarValue("var2").setReadable(true);
	keeper.getVarValue("var1").setReadable(false);
	BOOST_CHECK_NO_THROW(translator.translate("gtel var1, arr, var2"));
	keeper.getVarValue("var1").setWriteable(false);
	BOOST_CHECK_THROW(translator.translate("gtel var1, arr, var2"), ParseError);
}



/**
Тест трансляции команды STEL
*/

BOOST_AUTO_TEST_CASE(CodeTranslatorTest_stel)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate("stel v, arr, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("lbl: stel tmp , str, n ; "));
	BOOST_CHECK_NO_THROW(translator.translate("lbl: stel tmp , str   , n; "));
	BOOST_CHECK_NO_THROW(translator.translate("lbl:stel 	tmp,str,n"));
	BOOST_CHECK_NO_THROW(translator.translate("lbl:stel 	tmp,str,1"));
	BOOST_CHECK_NO_THROW(translator.translate("stel v[1], arr, 1"));


	BOOST_CHECK_THROW(translator.translate("lbl:stel k,1,n"), ParseError);


	DataKeeper keeper;
	keeper.addVar(Value(125, Value::MOD16, true, false), "var1");
	keeper.addVar(Value(100, Value::SIGNED_INT, true, false), "var2");
	keeper.addVar(Value(1, Value::UNSIGNED_INT, false, true), "var3");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(1, Value::MOD16, false, false);
	array[2];

	keeper.addArray(array, "arr");
	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_NO_THROW(translator.translate("stel var1, arr, 1"));

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::STEL);


	boost::shared_ptr<ArrayOperand> parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(parr -> getArrayPtr(), &keeper.getArray("arr"));

	boost::shared_ptr<VarOperand> op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(2));
	BOOST_CHECK_EQUAL(op -> getValue(), 1);


	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));
	BOOST_CHECK_EQUAL(op -> hasValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue(), 125);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), false);


	BOOST_CHECK_NO_THROW(translator.translate("stel var1, arr, var2"));

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::STEL);

	parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(parr -> getArrayPtr(), &keeper.getArray("arr"));

	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(2));
	BOOST_CHECK_EQUAL(op -> getValue(), 100);


	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));
	BOOST_CHECK_EQUAL(op -> hasValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue(), 125);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), false);


	BOOST_CHECK_THROW(translator.translate("stel var2, arr, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("stel var2, arr, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("stel var1, arr, var3"), ParseError);
	BOOST_CHECK_THROW(translator.translate("stel var1, arr, var4"), ParseError);

	keeper.getVarValue("var2").setReadable(false);
	BOOST_CHECK_THROW(translator.translate("stel var1, arr, var2"), ParseError);
	keeper.getVarValue("var2").setReadable(true);
	keeper.getVarValue("var1").setReadable(false);
	BOOST_CHECK_THROW(translator.translate("stel var1, arr, var2"), ParseError);
	keeper.getVarValue("var1").setReadable(true);

	keeper.getVarValue("var3").setReadable(true);

	BOOST_CHECK_NO_THROW(translator.translate("stel (mod16)var3, arr, var1"));
	parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(parr -> getArrayPtr(), &keeper.getArray("arr"));

	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(2));
	BOOST_CHECK_EQUAL(op -> getValue(), 125);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), false);

	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));
	BOOST_CHECK_EQUAL(op -> getValue(), 1);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), false);


}



BOOST_AUTO_TEST_CASE(CodeTranslatorTest_size)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate("size var, arr"));
	BOOST_CHECK_NO_THROW(translator.translate("size var,a;comment"));
	BOOST_CHECK_THROW(translator.translate("size var,1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("size 1,arr"), ParseError);


	DataKeeper keeper;
	keeper.addVar(Value(125, Value::MOD16, false, true), "var1");
	keeper.addVar(Value(100, Value::SIGNED_INT, true, true), "var2");
	keeper.addVar(Value(1, Value::UNSIGNED_INT, true, false), "var3");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(1, Value::MOD16, false, false);
	array[2];

	keeper.addArray(array, "arr");
	translator.setDataKeeperPtr(&keeper);

	BOOST_CHECK_NO_THROW(translator.translate("size var1, arr"));

	BOOST_CHECK_EQUAL(translator.getCommand().getOperationType(), Command::SIZE);

	boost::shared_ptr<ArrayOperand> parr = boost::dynamic_pointer_cast<ArrayOperand, Operand>(translator.getCommand().getOperand(1));
	BOOST_CHECK_EQUAL(parr -> getArrayPtr(), &keeper.getArray("arr"));

	boost::shared_ptr<VarOperand> op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));

	BOOST_CHECK_EQUAL(op -> hasValue(), true);
	BOOST_CHECK_EQUAL(op -> getValue(), 125);
	BOOST_CHECK_EQUAL(op -> getAfterCastType(), Value::MOD16);
	BOOST_CHECK_EQUAL(op -> isReadable(), true);
	BOOST_CHECK_EQUAL(op -> isWriteable(), true);

	BOOST_CHECK_NO_THROW(translator.translate("size var2, arr"));

	op = boost::dynamic_pointer_cast<VarOperand, Operand>(translator.getCommand().getOperand(0));
	BOOST_CHECK_EQUAL(op -> getValue(), 100);

	BOOST_CHECK_THROW(translator.translate("size var3, arr"), ParseError);
	BOOST_CHECK_NO_THROW(translator.translate("size arr[0], arr"));

}





BOOST_AUTO_TEST_CASE(CodeTranslatorTest_shifts)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate("shl a, b"));
	BOOST_CHECK_NO_THROW(translator.translate("shl a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("shr a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("shrwo a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("shlwo a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("rol a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("ror a, 1"));

	BOOST_CHECK_THROW(translator.translate("ror 1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("ror a"), ParseError);
	BOOST_CHECK_THROW(translator.translate("ror 1"), ParseError);


	DataKeeper keeper;
	keeper.addVar(Value(125, Value::MOD16, false, true), "var1");
	keeper.addVar(Value(100, Value::SIGNED_INT, true, true), "var2");
	keeper.addVar(Value(1, Value::UNSIGNED_INT, true, false), "var3");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(1, Value::MOD16, false, false);
	array[2];

	keeper.addArray(array, "arr");
	translator.setDataKeeperPtr(&keeper);


	BOOST_CHECK_NO_THROW(translator.translate("shl var2, var3"));
	BOOST_CHECK_NO_THROW(translator.translate("shr var2, var2"));
	BOOST_CHECK_NO_THROW(translator.translate("ror var2, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("rol var2, arr[0]"));
	BOOST_CHECK_NO_THROW(translator.translate("shlwo arr[0], var2"));
	BOOST_CHECK_NO_THROW(translator.translate("shrwo arr[0], var3"));
	BOOST_CHECK_NO_THROW(translator.translate("shl var2, 0"));

	BOOST_CHECK_THROW(translator.translate("shl var1, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("shl var3, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("shl arr, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("shl 0, var2"), ParseError);
}




BOOST_AUTO_TEST_CASE(CodeTranslatorTest_bits)
{
	CodeTranslator translator;

	BOOST_CHECK_NO_THROW(translator.translate("and a, b"));
	BOOST_CHECK_NO_THROW(translator.translate("xor a, b"));
	BOOST_CHECK_NO_THROW(translator.translate("or a, b"));
	BOOST_CHECK_NO_THROW(translator.translate("or a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("and a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("xor a, 1"));
	BOOST_CHECK_NO_THROW(translator.translate("and a[1], b"));
	BOOST_CHECK_NO_THROW(translator.translate("xor a[0], b"));
	BOOST_CHECK_NO_THROW(translator.translate("or a[3], b"));
	BOOST_CHECK_NO_THROW(translator.translate("and a[1], b[0]"));
	BOOST_CHECK_NO_THROW(translator.translate("xor a[0], b[0]"));
	BOOST_CHECK_NO_THROW(translator.translate("or a[3], b[0]"));
	BOOST_CHECK_NO_THROW(translator.translate("not a"));
	BOOST_CHECK_NO_THROW(translator.translate("not a[0]"));



	BOOST_CHECK_THROW(translator.translate("or (mod8) a, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or 1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or 1, a"), ParseError);
	BOOST_CHECK_THROW(translator.translate("xor 1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("and 1, 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or a"), ParseError);
	BOOST_CHECK_THROW(translator.translate("and 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("not 1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("not (mod8) a"), ParseError);


	DataKeeper keeper;
	keeper.addVar(Value(125, Value::MOD16, false, true), "var1");
	keeper.addVar(Value(100, Value::MOD16, true, true), "var2");
	keeper.addVar(Value(1, Value::MOD16, true, false), "var3");
	keeper.addVar(Value(1, Value::UNSIGNED_INT, true, true), "var4");

	Array array(3, Value::MOD16);
	array[0] = Value(1, Value::MOD16, true, true);
	array[1] = Value(1, Value::MOD16, false, false);
	array[2];

	keeper.addArray(array, "arr");
	translator.setDataKeeperPtr(&keeper);



	BOOST_CHECK_NO_THROW(translator.translate("and var2, var3"));
	BOOST_CHECK_NO_THROW(translator.translate("and var4, (uint) var3"));
	BOOST_CHECK_NO_THROW(translator.translate("or var2, var3"));
	BOOST_CHECK_NO_THROW(translator.translate("or var4, (uint) var3"));
	BOOST_CHECK_NO_THROW(translator.translate("xor var2, var3"));
	BOOST_CHECK_NO_THROW(translator.translate("xor var4, (uint) var3"));
	BOOST_CHECK_NO_THROW(translator.translate("not var2"));
	BOOST_CHECK_NO_THROW(translator.translate("not var4"));
	BOOST_CHECK_NO_THROW(translator.translate("not arr[0]"));



	BOOST_CHECK_THROW(translator.translate("and var1, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("and var3, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("and arr, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("and 0, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or var1, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or var3, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or arr, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("or 0, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("xor var1, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("xor var3, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("xor arr, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("xor 0, var2"), ParseError);
	BOOST_CHECK_THROW(translator.translate("not var1"), ParseError);
	BOOST_CHECK_THROW(translator.translate("not var3"), ParseError);
	BOOST_CHECK_THROW(translator.translate("not arr[1]"), ParseError);
}


BOOST_AUTO_TEST_SUITE_END();
