#ifndef CALL_OPERAND_H_
#define CALL_OPERAND_H_


/**
@file CallOperand 
*/

#include <list>


#include "Operand.h"
#include "DataKeeper.h"
#include "Value.h"
#include "Array.h"


/**
@class CallOperand
@brief Предназначен для представление операндов команды call
*/

class CallOperand: public Operand
{
	public:
/**
Конструктор по-умолчанию
*/
		CallOperand();

/**
Деструктор
*/
		~CallOperand();

/**
Установка значения.
@param val - добавляемое значение
*/
		void setValue(const Value &val);


/**
Установка массива
@param arr - добавляемый массив
*/
		void setArray(const Array &arr);


/**
Проверка на тип
*/
		bool isValue() const;



/**
Проверка на тип
*/
		bool isArray() const;



/**
Взятие значения у операнда. Имеет смысл только при isValue() == true;
*/
		Value getValue() const;


/**
Взятие массива у операнда. Имеет смысл только при isArray() == true;
*/
		Array getArray() const;


	private:
		Value 	m_val;
		Array	m_arr;
		bool	m_fisValue;
};


#endif
