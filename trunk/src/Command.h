#ifndef COMMAND_H_
#define COMMAND_H_



/**
@file Command.h
*/

#include <vector>

#include <boost/algorithm/string/case_conv.hpp>

#include "Operand.h"



/**
@class Command
@brief Класс, предназначенный для представления команды.
*/


class Command
{
	public:
/** 
@brief Перечисление типов комманд.
*/
	enum Operation
	{
		MOV = 1,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		JMP,
		JL,
		JE,
		JG,
		JNE,
		OUT,
		RSZ,
		CALL,
		RET,
		NOP,
		CMP,
		NONE
	};

/**
Конструктор по умолчанию.
*/
		Command();


/**
Конструктор от типа команды.
*/
		Command(Command::Operation operation);


/**
Деструктор.
*/
		~Command();


/**
Установка типа команды.
@param operation - тип команды
*/
		void setOperationType(Command::Operation operation);

/**
Получение типа команды.
@return - тип команды
*/
		Command::Operation getOperationType() const;


/**
Установка первого операдна комадны.
@param operand - первый операнд команды
*/
		void setFirstOperand(const Operand &operand);


/**
Установка второго операнда комадны.
@param operand - второй операнд команды
*/
		void setSecondOperand(const Operand &operand);


/**
Преобразование строковой записи команды в форму Command::Operation
@param str - строковое представление команды
@return представление команды в формате Command::Operation
*/

		static Command::Operation strToOperation(const std::string &str);

	private:
		Command::Operation 			m_opType;
		std::vector<Operand>		m_operands;
};


#endif
