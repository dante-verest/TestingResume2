#pragma once
#include <fstream>
#include <string>

namespace Files
{
	template<typename SequenceContainer>
	class FileParser
	{
	private:
		std::fstream m_file;
	public:
		// ctors
		FileParser();
		FileParser(const char* aFileName,
			std::ios::openmode mode = std::ios_base::in | std::ios_base::out);
		// dtor
		~FileParser();

		// открытие файла
		void OpenFile();
		// закрытие файла
		void CloseFile();
		// вставить данные в файл
		void SetDataToFile();
		// получить данные из файла
		void GetDataFromFile();
		// удалить слово в содержимом файла
		void DeleteWord(const char* aWord);
		// отсортировать строки содержимого файла
		void SortFileStrings(bool aIsAscending = true);
	};
}