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
		void setValuePtr(Value *pval);


/**
Установка массива
@param arr - добавляемый массив
*/
		void setArrayPtr(Array *parr);


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
Взятие числового значения операнда. Используется при передаче не по ссылке
@return числовое значение операнда
*/
//		long long getVarValue() const;

/**
Установка типа, к которому приведётся операнд. Используется при передаче не по ссылке
*/

//		void setCastType(Value::ValueType type);

/**
Взятие массива у операнда. Имеет смысл только при isArray() == true;
*/
		Array getArray() const;


/**
Является ли значение элементом массива?
@return true - да
*/
		bool isArrayElement() const;


/**
Установка операндом элемента массива. Может использоваться для массивов неподходящей длинный.
@param parr - указатель на массив
@param indx - смещение элемента в массиве
*/

		void setArrayElement(Array *parr, std::size_t indx);

/**
Строит объект для нового хранителя данных по старому.
@param pold - старый хранитель
@param pnew - ноый хранитель
@return операнд
*/
		CallOperand convert(const DataKeeper *pold, DataKeeper *pnew) const;


/**
Получение типа операнда. Имеет смысл, если операнд - переменная.
@return тип операнда
*/
		Value::ValueType getValueType() const;

/**
Проверка на то, имеется ли значение у объекта
*/
		bool hasValue() const;

/**
Установка типа операнда. Используется для установки типа числового значения. Имеет смысл только при isValue() == true
@param type - устанавливаемый тип
*/
		void setValueType(Value::ValueType type); 

/**
Замена старых адресов новыми
*/
		void replace(Array *pold, Array *pnew);

/**
Установка права на запись
*/
		void setWriteable(bool writeable);

/**
Проверка права на чтение
*/
		bool isWriteable() const;

	private:
		Value 				*m_pval;
		Array				*m_popArr;
		std::size_t			m_indx;
		Array 				*m_parr;
		Value::ValueType 	m_type;
		bool				m_writeable;
};


#endif
