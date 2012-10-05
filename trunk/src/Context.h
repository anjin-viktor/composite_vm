#ifndef CONTEXT_H_
#define CONTEXT_H_

/**
@file Context.h
*/

#include <vector>

#include "Function.h"
#include "Command.h"
#include "Exception.h"

/**
@class Context 
@brief Предназначен для представления исполняющейся функции
*/

class Context
{
	friend class CodeExecuter;
	public:
/**
@brief перечисление для представления результата команды cmp
*/
		enum CmpResult
		{
			Equal = 1,
			Greater = 2,
			NotDefined = 4
		};
/**
Конструктор.
*/
		Context();

/**
Оператор копирования. Нужен для безопасного копирования m_code
@param cntx
*/
		Context(const Context &cntx);
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
Получение константного указателя на функцию.
@return указатель на функцию объекта
*/
		Function *getFunctionPtr();


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
Оператор присвоения. Нужен для безопасного копирования m_code
@param cntx
@retval результать операции присвоения (ссылка на новое значение)
*/
		Context &operator =(const Context &cntx);



/**
Перейти к обработке исключительной ситуации определённого типа
@param exceptType - тип ислючительной ситуации
@throw std::runtime_error в случае отсутствия обработчика
*/
		void execHandler(Exception::Type);


/**
Возвращает тип выполняемого кода(какое обрабатывается исключение, либо код функции).
Exception::NoType - основной код модуля
@return тип выполняемого коде.
*/
		Exception::Type currentCode() const;

	private:
		Function 				m_function;
		std::size_t				m_ip;
		std::vector<Command>	m_code;
/*
Исполняемая часть. Exception::NoText - выполнение основного кода
*/
		Exception::Type 		m_currentCode;

		int 					m_cmpRes;
};


#endif
