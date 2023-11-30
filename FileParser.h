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

		// �������� �����
		void OpenFile();
		// �������� �����
		void CloseFile();
		// �������� ������ � ����
		void SetDataToFile();
		// �������� ������ �� �����
		void GetDataFromFile();
		// ������� ����� � ���������� �����
		void DeleteWord(const char* aWord);
		// ������������� ������ ����������� �����
		void SortFileStrings(bool aIsAscending = true);
	};
}