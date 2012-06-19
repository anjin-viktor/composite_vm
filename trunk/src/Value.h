#ifndef VALUE_H_
#define VALUE_H_

/**
@file Value.h
*/

#include <stdexcept>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include "Operand.h"


/**
@class Value 
@brief Класс, предназначенный для хранения значения переменной или константы в коде. Содержит в себе значение, его тип, права доступа.
*/

class Value
{
	public:

/** 
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
@throw std::runtime_error - в случае выхода параметра val за границы всех типов
*/
		Value(long long val) throw(std::runtime_error);

/**
Конструктор от значения и прав доступа.
@param val - параметр, указывающий значение
@param readable - есть ли право на чтение?
@param writeable - есть ли право на запись?
@throw std::runtime_error - в случае выхода параметра val за границы всех типов
*/
		Value(long long val, bool readable, bool writeable) throw(std::runtime_error);

/**
Конструктор от значения, типа и прав доступа .
@param val - параметр, указывающий значение
@param type - тип конструируемого значения
@param readable - есть ли право на чтение?
@param writeable - есть ли право на запись?
*/
		Value(long long val, ValueType type, bool readable, bool writeable);

/**
Виртуальный деструктор
*/
		virtual ~Value();


/**
Возвращаемое значение указывает, есть ли право на чтение значения.
@return право на чтение
*/
		bool isReadable() const;

/**
Возвращаемое значение указывает, есть ли право на запись значения.
@return право на запись
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
@return long long ячейку, содержащую значение
@throws std::runtime_error
*/
		long long getValue(ValueType toType = NO_TYPE) const throw(std::runtime_error);

/**
Установка значения из long long. При этом производится его приведение к типу объекта если он определён.
В противном случае он становится минимальным возможных для хранения указанного значения.
@param val - устанавливаемое значение
@throw std::runtime_error - в случае выхода параметра val за границы всех типов при неустановленом типе объекта
*/

		void setValue(long long val) throw(std::runtime_error);


/**
Преобразует запись сипа в виде строки к виду Value::ValueType
@param str - строковое представление типа
@return тип
*/
		static ValueType strToValueType(const std::string &str);

	private:
/**
Если значение не инициализировано указатель 
*/
		boost::shared_ptr<long long>						m_pval;
		ValueType											m_type;
		bool												m_isReadable;
		bool												m_isWriteable;


/**
Определяет минимальный тип, достаточных для хранения значения. Если значение не помещается ни в один из возможных типов
возвращает Value::NO_TYPE;
@param val - значение
@return минимальный тип
*/
		static ValueType minTypeForValue(long long val);

/**
Осуществляет приведение значения в ячейке long long к указанному типу
@param val - значение
@param type - требуемый тип
@return значение, приведённое к типу
@throws std::runtime_error в случае приведения к типу Value::NO_TYPE
*/
		static long long longlongToType(long long val, ValueType type) throw(std::runtime_error);

};

#endif
