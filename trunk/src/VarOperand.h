#ifndef VAR_OPERAND_H_
#define VAR_OPERAND_H_




#include <stdexcept>


#include "Operand.h"
#include "Value.h"



/**
@class VarOperand 
@brief Класс, предназначенный для предаставления переменных и числовых констант как операднов команды.
*/


class VarOperand: public Operand
{
	public:
/**
Конструктор по-умолчанию.
*/
		VarOperand();
	
/**
Конструктор от имени переменной.
@param name - имя переменной
@throw std::runtime_error - в случае несуществования переменной с таким именем
*/

		VarOperand(const std::string &name) throw(std::runtime_error);


/**
Конструктор от числа. Если не существует значения с соответсвующим числовым значением в рамках функции - оно добавляется.
@param value - числовое значения константы
*/

		VarOperand(long long value);

/**
Виртуальный деструктор
*/
		~VarOperand();





/**
Указание что команда использует переменную с переданным именем. Переменная соответствует объекту Value с соответствующим именем.
@param name - имя переменной
@throw std::runtime_error - в случае несуществования переменной с таким именем
*/
		void setVarName(const std::string &name) throw(std::runtime_error);

/**
Установка значения . Если не существует значения с соответсвующим числовым значением в рамках функции - оно добавляется.
@param value - числовое значения константы
*/
		void setLongLongValue(long long value);

/**
Указание типа, к которому приводим значение переменной входе выполнения команды.
@param type - указание целевого типа
*/
		void setType(Value::ValueType type);

	private:
		Value 				*m_pval;
		Value::ValueType	m_type;
};




#endif
