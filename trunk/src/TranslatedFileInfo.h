#ifndef TRANSLATED_FILE_INFO_H_
#define TRANSLATED_FILE_INFO_H_

/**
@file TranslatedFileInfo.h
*/

#include <string>
#include <cstdlib>


/**
@class TranslatedFileInfo
@brief Служит для хранения информации о транслируемом файле.
*/

class TranslatedFileInfo
{
	public:
/**
Получение ссылки на для использования функциональности класса. Шаблон Signleton.
*/
		static TranslatedFileInfo& getInstance();

/**
Получение имени транслируемого файла.
@return имя транслируемого файла
*/
		std::string getFile() const;

/**
Получение номера транслируемой строки.
@return номер строки
*/
		std::size_t getLineNo() const;


/**
Установка имени транслируемого файла.
@param name - устанавливаемое имя файла
*/
		void setFile(const std::string &name);

/**
Сброс позиции в файле.
*/
		void clearLineNo();

/**
Инкрементация номера строки в файле.
*/
		void incLineNo();

	private:
		TranslatedFileInfo();
		~TranslatedFileInfo();

		static TranslatedFileInfo 		m_instance;
		std::string						m_fileName;
		std::size_t						m_lineNo;
};


#endif
