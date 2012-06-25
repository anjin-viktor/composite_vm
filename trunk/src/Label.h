#ifndef LABEL_H_
#define LABEL_H_

/**
@file Label.h
*/


#include <string>
#include <cstddef>
#include <stdexcept>

#include <boost/regex.hpp>


/**
@class Label
@brief Используется для представления меток в ходе трансляции.
*/


class Label
{
	public:
/**
Конструктор по-умолчанию.
*/
		Label();
/**
Конструктор от строки - имени метки и её адреса. Имя метки должно удовлетворять накладываемым на него ограничениям. В противном случае
возбуждается исключение. Адресом является смещение метки, выраженное в числе команд относительно начала секции команд функции.
@param labelName - имя метки
@param addr - адрес метки
@throw std::logic_error - вслучае некорректной метки
*/
		Label(const std::string &labelName, std::size_t addr) throw(std::logic_error);

/**
Деструктор.
*/
		~Label();


/**
Установка имени метки. Имя метки должно удовлетворять накладываемым на него ограничениям. В противном случае
возбуждается исключение.
@param labelName - имя метки
@throw std::logic_error - вслучае некорректной метки
*/
		void setLabelName(const std::string &labelName) throw(std::logic_error);


/**
Установка адреса метки. Адресом является смещение метки, выраженное в числе команд относительно начала секции команд функции.
@param addr - устанавливаемый адрес метки
*/
		void setLabelAddr(std::size_t addr);

/**
Получение имени метки.
@return имя метки
*/
		std::string getLabelName() const;


/**
Получение адреса метки.
@return адресс метки.
*/
		std::size_t getLabelAddr() const;

	private:
		std::size_t		m_addr;
		std::string		m_labelName;
};



#endif
