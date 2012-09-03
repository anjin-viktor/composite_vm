#ifndef CALL_OPERAND_H_
#define CALL_OPERAND_H_


/**
@file CallOperand.h
*/

#include <list>
#include <string>

#include <iostream>

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

/**
Строит объект для нового хранителя данных по старому.
@param pold - старый хранитель
@param pnew - ноый хранитель
@return операнд
*/
	CallOperand convert(const DataKeeper *pold, DataKeeper *pnew) const;


/**
Установка типа операнда. Используется для установки типа числового значения. Имеет смысл только при isValue() == true
@param type - устанавливаемый тип
*/
	void setValueType(Value::ValueType type); 

	private:
		Value 	m_val;
		Array	m_arr;
		bool	m_fisValue;
};


#endif
