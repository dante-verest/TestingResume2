#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <clocale>
#ifdef _WIN32
#include <codecvt>
#endif

namespace Files
{
	//template<typename SequenceContainer>
	class FileParser
	{
	public:
		using String = std::string;
		using FileStream = std::fstream;
	private:
		char* m_pFileName;
		std::vector<String> m_fileData;
		FileStream m_file;
	public:
		// ctors
		FileParser() = default;
		FileParser(const char* aFileName,
			std::ios::openmode mode = std::ios_base::in | std::ios_base::out);
		// dtor
		~FileParser();

		// открытие файла
		void OpenFile(const char* aFileName, 
			std::ios::openmode mode = std::ios_base::in | std::ios_base::out);
		// закрытие файла
		void CloseFile();
		// вывести данные на экран
		void ShowFileData();
		// записать данные в файл из буфера
		void WriteToFile();
		// вставить новые данные в буфер
		//void SetDataToFile(std::string aNewString);
		// прочесть файл в буфер
		void ReadFromFile();
		// получить прочитанные данные файла из буфера
		//std::vector<std::string>& GetDataFromFile();
		// удалить слово в содержимом файла
		void DeleteWord(int argc, char* argv[]);
		// отсортировать строки содержимого файла
		void SortFileStrings(bool aIsAscending = true);
	private:
		bool IsYourFileOpen();
		bool IsYouReadTheFileData();
		String UTF8toString(const String& utf8str, const std::locale& loc);
	};
}