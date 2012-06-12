#ifndef VALUE_H_
#define VALUE_H_


/**
@file Valur.h
*/

/**
@class Value 
@brief Класс, предназначенный для хранения значения переменной или константы в коде. Содержит в себе значение, его тип, права доступа.
*/

class Value
{
	public:

/** @enum Value::ValyeType  
@brief Перечисление типов данных значений. Используется для обозначения типа данных объектов Value.
*/

		enum ValueType
		{
			///8 бит, беззнаковое
			UNSIGNED_CHAR = 1,
			///8 бит, знаковое
			SIGNED_CHAR,
			///8 бит, операции mod 256
			MOD8,
			///16 бит, беззнаковое
			UNSIGNED_SHORT,
			///16 бит, знаковое
			SIGNED_SHORT,
			///16 бит, операции mod 2^16
			MOD16,
			///32 бита, знаковое
			UNSIGNED_INT,
			///32 бита, знаковое
			SIGNED_INT,
			///32 бит, операции mod 2^32
			MOD32,
			///значение для указания на отсутствие типа
			NO_TYPE
		};


/**
Конструктор по-умолчанию. При его использовании требуется дальнейшая конкретизация объекта.
**/
		Value();
/**
Конструктор от значения. Переменная инициализируется указанным значением, её типом становится минимальных тип, покрывающий
это значение. Отсутсвует доступ на чтение и запись.
@param val - параметр, указывающий значение.
*/
		Value(long long val);

/**
Конструктор от значения и прав доступа.
@param val - параметр, указывающий значение
@param readable - есть ли право на чтение?
@param writeable - есть ли право на запись?
*/
		Value(long long val, bool readable, bool writeble);

/**
Конструктор от значения, типа и прав доступа .
@param val - параметр, указывающий значение
@param type - тип конструируемой переменной
@param readable - есть ли право на чтение?
@param writeable - есть ли право на запись?
*/
		Value(long long val, ValueType type, bool readable, bool writeable);

/**
Возвращаемое значение указывает, есть ли право на чтение значения.
*/
		bool isReadable() const;

/**
Возвращаемое значение указывает, есть ли право на запись значения.
*/
		bool isWriteable() const;


/**
Функция устанавливает право на чтение.
@param readable - есть ли право на чтение?
*/
		void setReadable(bool readable);
/**
Функция устанавливает право на запись
@param writeable - есть ли право на чтение?
*/
		void setWriteable(bool writeable);

/**
Установление типа значения. При этом происходит приведение к этому типу самого значения.
@param val - устанавливаемый тип
*/
		void setType(ValueType val);

/**
Получения типа значения.
@return Тип значения
*/
		ValueType getType() const;

/**
Получения значения в виде long long
@return long long ячейка, содержащая значение
*/
		long long getValue(ValyeType toType = NO_TYPE) const;

/**
Установка значения из long long. При этом производится его приведение к типу объекта если он определён.
В противном случае он становится минимальным возможных для хранения указанного значения.
*/

		void setValue(long long);

	private:
		long long 		m_val;
		ValueType		m_type;
		bool			isReadable;
		bool			isWriteable;


/**
Определяет минимальный тип, достаточных для хранения значения 
@param val - значение
@return - минимальный тип
*/
		static ValueType minTypeForValue(long long val);
};

#endif
