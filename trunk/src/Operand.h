#ifndef OPERAND_H_
#define OPERAND_H_


/**
@file Operand.h
*/

/**
@class Operand
@brief Абстрактный класс, являющийся базовым для операндов команды.
*/


class Operand
{
	public:
/**
Конструктор по-умолчанию
*/
		Operand() {};
/**
Виртуальный дестркутор
*/
		virtual ~Operand() {};
};

#endif
