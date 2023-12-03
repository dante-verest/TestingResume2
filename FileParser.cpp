#include "FileParser.h"
#include <iostream>
#include <algorithm>

Files::FileParser::FileParser() 
{
	if (m_file.is_open())
		m_file.close();
};
Files::FileParser::FileParser(const char* aFileName,
	std::ios::openmode mode) 
{
	if (m_file.is_open())
		m_file.close();
	else
		m_file = FileStream(aFileName, mode);
};

Files::FileParser::~FileParser()
{
	if (m_file.is_open())
		m_file.close();
#ifdef _WIN32
	std::wcout << L"Выполнено!" << std::endl;
#elif __linux__
	std::cout << "Выполнено!" << std::endl;
#endif
};

bool Files::FileParser::IsYourFileOpen()
{
	if (m_file.is_open())
		return true;
	else
	{
		std::cerr << "Is your file open?" << std::endl;
		return false;
	}
};
bool Files::FileParser::IsYouReadTheFileData()
{
	if (!m_fileData.empty())
		return true;
	else
	{
		std::cerr << "Is you read the file data? Maybe there is no data here?" << std::endl;
		return false;
	}
};

void Files::FileParser::OpenFile(const char* aFileName,
	std::ios::openmode mode)
{
	if (m_file.is_open())
	{
		m_file.close();
		m_file.open(aFileName, mode);
	}
	else
		m_file.open(aFileName, mode);

};
void Files::FileParser::CloseFile()
{
	if (m_file.is_open())
		m_file.close();
};
void Files::FileParser::ShowFileData()
{
	if (IsYouReadTheFileData())
	{
		std::cout << "File data:" << std::endl;
		for (auto& string : m_fileData)
			std::cout << '\t' << string << std::endl;
		}
	}
};
void Files::FileParser::Clear()
{
	if (IsYouReadTheFileData())
		m_fileData.clear();
};
void Files::FileParser::ReadFromFile()
{
	String fileString;
	if (IsYourFileOpen())
	{
		while (std::getline(m_file, fileString, '\n'))
		{
			m_fileData.push_back(fileString);
		}
	}
};
void Files::FileParser::WriteToFile()
{
	if(IsYourFileOpen() && IsYouReadTheFileData())
	for (const auto& string : m_fileData)
		m_file << string << std::endl;
};
void Files::FileParser::DeleteWords(int argc, const char** argv)
{
	String utf8str;
	std::size_t startPos = 0, i = 0, argvLength = 0;
	if (IsYouReadTheFileData())
	{
		for (auto fileString = m_fileData.begin(); fileString != m_fileData.end(); ++fileString)
		{
			if(argc > 1)
				for (i = 1; i < argc; i++)
				{
#ifdef _WIN32
					char str[100];
					wchar_t wstr[50];
					MultiByteToWideChar(1251, MB_PRECOMPOSED, argv[i], -1, wstr, sizeof(wstr) / sizeof(wstr[0]));
					WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, sizeof(str) / sizeof(str[0]), 0, 0);
					utf8str = str;
#elif __linux__
					utf8str = argv[i];
#endif
					argvLength = utf8str.length();
					startPos = fileString->find(utf8str);
					if (startPos == String::npos)
						continue;
					else
						while (startPos != String::npos)
						{
							if (std::isspace(fileString->at(startPos + argvLength)))
								fileString->erase(startPos, argvLength + 1);
							else
								fileString->erase(startPos, argvLength);
							startPos = fileString->find(utf8str, startPos + argvLength);
						};
				}

		}
	}
};
void Files::FileParser::DeleteEmptyStrings()
{
	if (IsYouReadTheFileData())
	{
		for (auto fileString = m_fileData.begin(); fileString != m_fileData.end(); )
			if (fileString->find_first_not_of(' ') == String::npos)
				fileString = m_fileData.erase(fileString);
			else
				++fileString;
	}
};
void Files::FileParser::SortFileStrings(bool aIsAscending)
{
	std::locale loc;
	if (IsYouReadTheFileData())
	{
#ifdef _WIN32
		loc = std::locale("");
		auto toLower = [](auto ch, const std::locale& loc) {
			return std::tolower<decltype(ch)>(ch, loc);
		};
		std::sort(m_fileData.begin(), m_fileData.end(), [&](const auto& element1, const auto& element2) {
			const auto result = std::mismatch(element1.cbegin(), element1.cend(), element2.cbegin(), element2.cend(),
			[&](const auto& char1, const auto& char2) {
					return toLower(char1, loc) == toLower(char2, loc);
				});
		if (aIsAscending)
			return (result.second != element2.cend()) && (result.first == element1.cend() ||
				toLower(*result.first, loc) < toLower(*result.second, loc));
		else
			return (result.second != element2.cend()) && (result.first == element1.cend() ||
				toLower(*result.first, loc) > toLower(*result.second, loc));
			});
#elif __linux__
		std::sort(m_fileData.begin(), m_fileData.end(), [&aIsAscending](const auto& element1, const auto& element2) {
			if (aIsAscending)
				return (strcasecmp(element1.c_str(), element2.c_str()) < 0);
			else
				return (strcasecmp(element1.c_str(), element2.c_str()) > 0);
			});
#endif
	}
};

