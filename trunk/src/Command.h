#ifndef COMMAND_H_
#define COMMAND_H_



/**
@file Command.h
*/

#include <vector>
#include <cstddef>
#include <string>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/shared_ptr.hpp>

#include "Operand.h"



/**
@class Command
@brief Класс, предназначенный для представления команды.
*/


class Command
{
	friend std::ostream &operator << (std::ostream &stream, const Command &);

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
		GTEL,
		STEL,
		SIZE,
		SHR,
		SHL,
		SHRWO,
		SHLWO,
		ROL,
		ROR,
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
Установка на операнд с указанным номером.
@param n - номер операнда
@param p - операнд команды
*/
		void setOperand(std::size_t n, boost::shared_ptr<Operand> p);


/**
Получение указателя на операнд с указанным номером.
@return указатель на операнд
*/
		boost::shared_ptr<Operand> getOperand(std::size_t n) const throw(std::out_of_range);




/**
Преобразование строковой записи команды в форму Command::Operation
@param str - строковое представление команды
@return представление команды в формате Command::Operation
*/


		static Command::Operation strToOperation(const std::string &str);



/**
Преобразование команды из формы Command::Operation в строковое представление.
@param op - операция
@return строковое представление операции
*/
		static std::string operationToStr(Command::Operation op);

/**
Установка положения строки команды в файле с исходным кодом. Используется при обработке ошибок.
*/
		void setLineNumber(std::size_t);


/**
Получение строки команды в файле исходного кода. Используется для сообщения об ошибках в обрабатываемой программе.
*/
		std::size_t getLineNumber() const;



/**
Оператор проверки на неравенство
*/
		bool operator != (const Command &command);


/**
Получение числа операндов команды. Имеет смысл только при их числе большем 2.
Необходимо для работы с операндами команды call.
@return число операндов
*/
		std::size_t getNumberOfOperands() const;

	private:
		Command::Operation 						 		m_opType;
		std::vector<boost::shared_ptr<Operand> >		m_operands;
		std::size_t										m_numb;
};


std::ostream &operator << (std::ostream &stream, const Command &comm);


#endif
