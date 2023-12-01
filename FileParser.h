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

		// �������� �����
		void OpenFile(const char* aFileName, 
			std::ios::openmode mode = std::ios_base::in | std::ios_base::out);
		// �������� �����
		void CloseFile();
		// ������� ������ �� �����
		void ShowFileData();
		// �������� ������ � ���� �� ������
		void WriteToFile();
		// �������� ����� ������ � �����
		//void SetDataToFile(std::string aNewString);
		// �������� ���� � �����
		void ReadFromFile();
		// �������� ����������� ������ ����� �� ������
		//std::vector<std::string>& GetDataFromFile();
		// ������� ����� � ���������� �����
		void DeleteWord(int argc, char* argv[]);
		// ������������� ������ ����������� �����
		void SortFileStrings(bool aIsAscending = true);
	private:
		bool IsYourFileOpen();
		bool IsYouReadTheFileData();
		String UTF8toString(const String& utf8str, const std::locale& loc);
	};
}