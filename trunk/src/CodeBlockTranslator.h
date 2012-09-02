#ifndef CODE_BLOCK_TRANSLATOR_H_
#define CODE_BLOCK_TRANSLATOR_H_

/**
@file CodeBlockTranslator.h
*/



#include <cstddef>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include "Command.h"
#include "Operand.h"
#include "LabelOperand.h"
#include "DataKeeper.h"
#include "CodeTranslator.h"
#include "ParseError.h"

/**
@class CodeBlockTranslator
@brief Используется для трансляции блоков кода. Блоками являются последовательности команд функций, 
обработчиков ислючительных ситуаций.
*/


class CodeBlockTranslator
{

	public:
/**
Конструктор по-умолчанию
*/
		CodeBlockTranslator();


/**
Конструктор от указателя на набор данных функции. Набор данных используется при встрече переменных или констант при транслировании
кода.
@param keeper - устанавливаемый набор данных
*/
		CodeBlockTranslator(DataKeeper *keeper);


/**
Деструктор.
*/
		~CodeBlockTranslator();

/**
Установка указателя на набор данных функций, блок которой транслируется.
@param keeper - устанавливаемый набор данных
*/
		void setDataKeeperPtr(DataKeeper *keeper);


/**
Трансляция строки.
@param str - транслируемая строка
@param lineNo - номер строки в исходном файле
*/
		void translate(const std::string &str, std::size_t lineNo = 0);


/**
Получение вектор команд - результата трансляции. Использовать только если все используемые в оттранслированном коде
метки были объявлены.
@return вектор команд - результат трансляции
*/
		std::vector<Command> getCommands();


/**
Очистка. Используется перед трансляцией нового блока кода.
*/
		void clear();

	private:
/**
Устанавливает у операндов всех переходов смещение целевой команды 
*/
		void processJmpCommands();

		CodeTranslator 							m_codeTranslator;
		std::vector<Command>					m_vcommands;
		std::map<int, std::list<std::string> >	m_mapLabels;
};


#endif
