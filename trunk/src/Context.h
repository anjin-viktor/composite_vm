#ifndef CONTEXT_H_
#define CONTEXT_H_

/**
@file Context.h
*/

#include <vector>

#include "Function.h"
#include "Command.h"


/**
@class Context 
@brief Предназначен для представления исполняющейся функции
*/

class Context
{
	friend class CodeExecuter;
	public:
/**
Конструктор.
*/
		Context();
/**
Деструктор.
*/
		~Context();

/**
Установка функции. 
@param f - устанавливаемая функция
*/
		void setFunction(const Function &f);
/**
Инициализация. Производить перед началом работы
*/
		void init();

/**
Дошли ли до конца кода блока.
@return true - команды больше нет
*/
		bool atEnd() const;

/**
Имеется ли обработчик на ислючительную ситуацию определённого типа
@param exceptType - тип ислючительной ситуации
@return true - есть обработчик
*/
		bool handlerIsExists(Exception::Type) const;


/**
Перейти к обработке исключительной ситуации определённого типа
@param exceptType - тип ислючительной ситуации
@throw std::runtime_error в случае отсутствия обработчика
*/
		void execHandler(Exception::Type);


	private:
		Function 				m_function;
		std::size_t				m_ip;
		std::vector<Command>	m_code;
};


#endif
