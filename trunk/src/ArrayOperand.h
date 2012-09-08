#ifndef ARRAY_OPERAND_H_
#define ARRAY_OPERAND_H_



/**
@file ArrayOperand.h
*/

#include "DataKeeper.h"
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

/**
Указывает, связан ли операнд с некоторым массивом.
@return операнд связан с массивом?
*/
		bool hasValue() const;


/**
Строит объект для нового хранителя данных по старому.
@param pold - старый хранитель
@param pnew - ноый хранитель
@return массив-операнд
*/
	ArrayOperand convert(const DataKeeper *pold, DataKeeper *pnew) const;

/**
Замена старых адресов новыми
*/
		void replace(Array *pold, Array *pnew);



	private:
		Array 		*m_parr;
};



#endif
