#ifndef CODE_BLOCK_TRANSLATOR_H_
#define CODE_BLOCK_TRANSLATOR_H_

/**
@file CodeBlackTranslator.CODE_BLOCK_TRANSLATOR_H_
*/



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
@brien Используется для трансляции блоков кода. Блоками являются последовательности команд функций, 
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
*/
		CodeBlockTranslator(DataKeeper *);


/**
Деструктор.
*/
		~CodeBlockTranslator();

/**
Установка указателя на набор данных функций, блок которой транслируется.
*/
		void setDataKeeperPtr(DataKeeper *);


/**
Трансляция строки.
*/
		void translate(const std::string &);


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
