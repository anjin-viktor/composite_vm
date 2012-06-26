#ifndef ARRAY_H_
#define ARRAY_H_

/**
@file Array.h
*/

#include <cstddef>
#include <vector>

#include <boost/shared_ptr.hpp>


#include "Value.h"




/**
@class Array
@brief Предназначен для представления массивов в рамках архитектуры Композит.
*/


class Array
{
	public:
/**
Конструктор по-умолчанию.
*/
		Array();
/**
Конструктор от числа элементов массива.
@param size - число элементов
*/
		Array(std::size_t size);

/**
Конструктор от числа элементов массива и их типа.
*/
		Array(std::size_t size, Value::ValueType type);

/**
Деструктор.
*/
		~Array();

/**
Устанавливает тип элементов массива.
@param type - тип элементов
*/
		void setType(Value::ValueType type);

/**
Изменяет размер массива. Значения элементов сохраняются.
@param newSize - новый размер массива
*/
		void resize(std::size_t newSize);

/**
Устанавливает право на перезапись массива. Право на перезапись позволяет изменять размер массива.
@param writeable - значение устанавливаемого права
*/
		void setWriteable(bool writeable);


/**
Устанавливает право на чтение для всех переменных массива.
@param readable - право на чтение
*/
		void setReadableAll(bool readable);

/**
Установление право на запись для элементов массива.
@param writeable - право на запись
*/
		void setWriteableAll(bool writeable);


/**
Возвращает размер массива.
@return размер массива
*/
		std::size_t size() const;


/**
Возвращает право на изменение массива.
@return право на изменение
*/
		bool isWriteable() const;



/**
Оператор константного доступа к элементам массива.
@param n - номер элемента
@return константную ссылку на элемент
@throw std::runtime_error - в случае выхода за границу массива
*/
		const Value& operator[] (std::size_t n) const;


/**
Оператор доступа к элементам массива.
@param n - номер элемента
@return ссылку на элемент
@throw std::runtime_error - в случае выхода за границу массива
*/

		Value& operator[] (std::size_t n);

	private:
		boost::shared_ptr<std::vector<Value> >		m_parr;
		bool										m_isWriteable;
		Value::ValueType							m_type;
};



#endif
