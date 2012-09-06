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
Установка смещение целевой команды относительно начала модуля. Используется после анализа модуля по имени, полученному изначально.
*/
		void setCommandOffset(std::size_t);

/**
Получение смещение целевой  командуымодуля. Используется, в первую очередь, при выполнении кода.
@return смещение целевой команды.
*/
		std::size_t getCommandOffset() const;

/**
Проверка строки на удовлетворение ограничениям, накладываемым на имя метки.
@param labelName - проверяемая строка
@return входная строка может являться именем метки?
*/
		static bool checkLabelName(const std::string &labelName);

	private:
/**
Смещение целевой команды относительно начала блока команд (блок команд - тело функции или облаботчика исключительной ситации).
*/
		std::size_t		m_offset;
		std::string		m_labelName;
};


#endif
