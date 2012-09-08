#ifndef VAR_OPERAND_H_
#define VAR_OPERAND_H_


#include <iostream>



#include <stdexcept>


#include "Operand.h"
#include "Value.h"
#include "Array.h"
#include "DataKeeper.h"


/**
@class VarOperand 
@brief Класс, предназначенный для предаставления переменных и числовых констант как операднов команды.
*/


class VarOperand: public Operand
{
	public:
/**
Конструктор от указателя на переменную, связанную с операндом и типа, к которому она приводится.
@param pval - указатель на переменную
@param type - тип, к которой приводится переменная при выполнении
*/
		VarOperand(Value *pval = NULL, Value::ValueType type = Value::NO_TYPE);

/**
Конструктор от указателя на массив, индекса элемента и типа, к которому он приводится.
@param parr - указатель на массив
@param n - индекс элемента в массиве
@param type - тип, к которой приводится переменная при выполнении
*/

		VarOperand(Array *parr, std::size_t n, Value::ValueType type);


/**
Виртуальный деструктор
*/
		~VarOperand();

/**
Установка указателя на переменную, связанную с операндом
@param pval - указатель на переменную
*/
		void setValuePtr(Value *pval);


/**
Построение объекта на новом наборе данных по старому
@param pold - старый объект хранитель, в котором ведётся поиск. 
@param pnew - новый объект хранитель. 
@return операнд относительно нового набора данных
*/
		VarOperand convert(const DataKeeper *pold, DataKeeper *pnew) const;


/**
Указывает связан ли с каким-либо значением этот операнд
@return true - связан с некоторым значением.
*/
		bool hasValue() const;


/**
Получение значения операнда на текущий момент времени.
@return значение операнда
*/
		long long getValue() const;
	
/**
Установка значения операнда. Используется для установки значения связанной с ним переменной.
@param val устанавливаемое значение
*/
		void setValue(long long val); 

/**
Установка указателя на массив, связанный с операдном и индекса элемента в нём.
@param parr - указатель на массив
@param n - индект элемента
*/
		void setArrayElementPtr(Array *parr, std::size_t n);


/**
Установка константности операнда.
@param constancy - устанавливаемое значение
*/
		void setConstancy(bool constancy);


/**
Проверка операнда на константность.
@return - true - операнд - константа, false - не константа
*/

		bool isConstant() const;



/**
Возвращаемое значение указывает, есть ли право на чтение операнда.
@return право на чтение
*/
		bool isReadable() const;

/**
Возвращаемое значение указывает, есть ли право на запись операнда.
@return право на запись
*/
		bool isWriteable() const;


/**
Указание типа, к которому приводим значение переменной входе выполнения команды.
@param type - указание целевого типа
*/
		void setType(Value::ValueType type);


/**
Отвечает на вопрос, осуществляется ли приведение типа при выполнении VarOperand::getValue()
*/
		bool isCast() const;


/**
Получение типа, к которому осуществляется приведения значения входе выполнения операции.
@return тип к которому приводится переменная
*/
		Value::ValueType getType() const;


/**
Полечение типа переменной после приведения.
@return тип переменной
*/
		Value::ValueType getAfterCastType() const;


/**
Выполнение инициализации. Осуществляется при первом использовании на запись значения. После этого переменная
становится читаемой.
*/
		void initialize();

/**
Замена старых адресов новыми
*/
		void replace(Value *pold, Value *pnew);
		void replace(Array *pold, Array *pnew);

	private:
		Value 				*m_pval;
		Value::ValueType	m_type;
		Array 				*m_parr;
		std::size_t			m_indx;
		bool				m_const;
};




#endif
