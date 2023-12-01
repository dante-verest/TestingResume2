#include "FileParser.h"
#include <iostream>
#include <algorithm>
#include <cstring>

Files::FileParser::FileParser() 
	: m_pFileName(nullptr)
	, m_file()
{
	std::setlocale(LC_ALL, "russian");
};
Files::FileParser::FileParser(const char* aFileName,
	std::ios::openmode mode = std::ios_base::in | std::ios_base::out) 
	: m_pFileName(aFileName)
	, m_file(aFileName, mode)
{
	std::setlocale(LC_ALL, "russian");
};

Files::FileParser::~FileParser()
{
	if (m_file.is_open())
		m_file.close();
	std::cout << "Выполнено!" << std::endl;
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
Files::FileParser::String UTF8toString(const Files::FileParser::String& utf8str, const std::locale& loc)
{
	// UTF-8 to wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr = wconv.from_bytes(utf8str);
	// wstring to string
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}


void Files::FileParser::OpenFile(const char* aFileName, 
	std::ios::openmode mode = std::ios_base::in | std::ios_base::out)
{
	this->FileParser::FileParser(aFileName, mode);
};
void Files::FileParser::CloseFile()
{
	if (m_file.is_open())
		m_file.close();
};
void Files::FileParser::ShowFileData()
{
	if (IsYouReadTheFileData())
		for (const auto& string : m_fileData)
			std::cout << string << std::endl;
};
void Files::FileParser::ReadFromFile()
{
	std::string fileString;
	if (IsYourFileOpen())
	{
		while (std::getline(m_file, fileString, '\n'))
			m_fileData.push_back(fileString);
	}
};
void Files::FileParser::WriteToFile()
{
	if(IsYourFileOpen() && IsYouReadTheFileData())
	for (const auto& string : m_fileData)
		m_file << string << std::endl;
};
void Files::FileParser::DeleteWord(int argc, char* argv[])
{
	std::size_t startPos = 0, i = 0, argvLength = 0;
	if (IsYouReadTheFileData())
	{
		for (const auto& fileString : m_fileData)
		{
			// предполагается, что файл в Linux и Windows записан в UTF-8
#ifdef _WIN32
			fileString = utf8_to_string(fileString, std::locale(".1251"));
#endif
			if (argc > 1)
			{
				for (i = 0; i < argc; i++)
				{
					argvLength = strnlen(argv[i], 100);
					startPos = fileString.find(argv[i]);
					if (startPos == String::npos)
						continue;
					else
						while (startPos != String::npos)
						{
							if (std::isspace(fileString.at(startPos + argvLength)))
								fileString.erase(startPos, argvLength + 1);
							else
								fileString.erase(startPos, argvLength);
							startPos = fileString.find(argv[i], startPos + argvLength);
						};
				}
			}
			if (fileString.find_first_not_of(' ') != String::npos)
				fileData.push_back(fileString);
		}
	}
};
void Files::FileParser::SortFileStrings(bool aIsAscending)
{
	if (IsYouReadTheFileData())
	{
		std::sort(m_fileData.begin(), m_fileData.end(), [](const auto& element1, const auto& element2) {
			const auto result = std::mismatch(element1.cbegin(), element1.cend(), element2.cbegin(), element2.cend(),
			[](const auto& char1, const auto& char2) {
					bool isTrue = std::tolower(char1) == std::tolower(char2);
					return isTrue;
				});
		return (result.second != element2.cend()) && (result.first == element1.cend() ||
			std::tolower(*result.first) < std::tolower(*result.second));
			});
	}
};

