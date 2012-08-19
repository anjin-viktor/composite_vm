#ifndef ARRAY_OPERAND_H_
#define ARRAY_OPERAND_H_



/**
@file ArrayOperand.h
*/

#include "Operand.h"
#include "Array.h"


/**
@class ArrayOperand
@brief Предназначен для представления массива - операнда.
*/
class ArrayOperand: public Operand 
{
	public:

/**
Конструктор от указателя на массив, используемого в качестве операнда.
@param parr - указатель на массив
*/
		ArrayOperand(Array *parr = NULL);

/**
Деструктор.
*/
		~ArrayOperand();


/**
Установка указателя на массив, используемый в качестве операнда.
@param parr - указатель на массив
*/
		void setArrayPtr(Array *parr);

/**
Получение указателя на массив, связанного с объектом.
@return указатель на массив
*/
		Array *getArrayPtr() const;

	private:
		Array 		*m_parr;
};



#endif