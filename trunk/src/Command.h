#ifndef COMMAND_H_
#define COMMAND_H_



/**
@file Command.h
*/

#include <vector>

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
*/
		void setOperationType(Command::Operation operation);

/**
Получение типа команды.
*/
		Command::Operation getOperationType() const;


/**
Установка первого операдна комадны.
*/
		void setFirstOperand(const Operand &operand);


/**
Установка второго операдна комадны.
*/
		void setSecondOperand(const Operand &operand);

	private:
		Command::Operation 			m_opType;
		std::vector<Operand>		m_operands;
};


#endif
