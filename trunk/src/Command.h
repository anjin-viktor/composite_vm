#ifndef COMMAND_H_
#define COMMAND_H_



/**
@file Command.h
*/

#include <vector>
#include <cstddef>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/shared_ptr.hpp>

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
		AOUT,
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
@param poperand - указатель на первый операнд команды
*/
		void setFirstOperand(boost::shared_ptr<Operand> poperand);


/**
Установка второго операнда комадны.
@param poperand - второй операнд команды
*/
		void setSecondOperand(boost::shared_ptr<Operand> poperand);


/**
Получение указателя на первый операнд команды.
@return указатель на первый операнд команды
@throw std::out_of_range при отсутствии первого операнда
*/

		boost::shared_ptr<Operand> getFirstOperand() const throw(std::out_of_range);
/**
Получение указателя на второй операнд команды.
@return указатель на второй операнд команды
@throw std::out_of_range при отсутствии второго операнда
*/


		boost::shared_ptr<Operand> getSecondOperand() const throw(std::out_of_range);

/**
Преобразование строковой записи команды в форму Command::Operation
@param str - строковое представление команды
@return представление команды в формате Command::Operation
*/

		static Command::Operation strToOperation(const std::string &str);

/**
Установка положения строки команды в файле с исходным кодом. Используется при обработке ошибок.
*/
		void setLineNumber(std::size_t);


/**
Получение строки команды в файле исходного кода. Используется для сообщения об ошибках в обрабатываемой программе.
*/
		std::size_t getLineNumber() const;


	private:
		Command::Operation 						 		m_opType;
		std::vector<boost::shared_ptr<Operand> >		m_operands;
		std::size_t										m_numb;
};


#endif
