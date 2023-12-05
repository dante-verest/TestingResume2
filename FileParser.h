#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <clocale>
#include <cstring>
#include <wchar.h>
#include <memory>
#ifdef _WIN32
#pragma warning(disable: 4996)
#include <codecvt>
#include <Windows.h>
#endif

namespace Files
{
	class FileParser
	{
	public:
		using String = std::string;
		using FileStream = std::fstream;
	private:
		std::vector<String> m_fileData;
		FileStream m_file;
	public:
		// ctors
		FileParser();
		FileParser(const char* aFileName,
			std::ios::openmode mode = std::ios_base::in | std::ios_base::out);
		// dtor
		~FileParser();

		// открытие файла
		void OpenFile(const char* aFileName, 
			std::ios::openmode mode = std::ios_base::in | std::ios_base::out);
		// закрытие файла
		void CloseFile();
		// очистить буфер
		void Clear();
		// вывести данные на экран
		void ShowFileData();
		// записать данные в файл из буфера
		void WriteToFile();
		// прочесть файл в буфер
		void ReadFromFile();
		// удалить пустые строки
		void DeleteEmptyStrings();
		// удалить слово в содержимом файла
		void DeleteWords(int argc, const char** argv);
		// отсортировать строки содержимого файла
		void SortFileStrings(bool aIsAscending = true);
	private:
		bool IsYourFileOpen();
		bool IsYouReadTheFileData();
	};
}