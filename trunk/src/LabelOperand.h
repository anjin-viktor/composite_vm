#ifndef LABEL_H_
#define LABEL_H_


/**
@file LabelOperand.h
*/

#include <string>
#include <boost/regex.hpp>

#include "Operand.h"

/**
@class LabelOperand 
@brief Класс, предназначенный для хранения и работы с метками. Используется как операнд класса Command.
*/

class LabelOperand: public Operand
{
	public:
/**
Конструктор по-умолчанию.
*/
		LabelOperand();

/**
Конструктор от имени метки.
@param label_name - имя метки.
*/
		LabelOperand(const std::string &label_name);


/**
Виртуальный деструктор.
*/
		virtual ~LabelOperand();

/**
Установка имени метки. Используется на первой стадии трансляции. После анализа всего модуля по имени устанавливается
указатель на команду.

@param label - имя метки.
*/
		void setLabelName(const std::string &label);

/**
Получение имени метки. После анализа модуля по имени устанавливается указатель на команду.
@return имя метки, сохранённое в объекте.
*/
		std::string getLabelName() const;

/**
Установка указателя на команду модуля. После анализа модуля по имени, полученному изначально,
устанавливается указатель на команду.
@return имя метки, сохранённое в объекте.
*/
		void setCommandPtr();

/**
Получение указателя на команду модуля. Используется, в первую очередь, при выполнении кода.
@return имя метки, сохранённое в объекте.
*/
		void getCommandPtr();

/**
Проверка строки на удовлетворение ограничениям, накладываемым на имя метки.
@param labelName - проверяемая строка
@return входная строка может являться именем метки?
*/
		static bool checkLabelName(const std::string &labelName);


	private:


		std::string		m_labelName;
};


#endif