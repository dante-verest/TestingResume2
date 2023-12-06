#include "FileParser.h"
#include "iconv.h"
#include <iostream>
#include <filesystem>
#include <codecvt>

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
#ifdef _WIN32
		std::wcerr << L"Is your file open?" << std::endl;
#elif __linux__
		std::cerr << "Is your file open?" << std::endl;
#endif
		return false;
	}
};
bool Files::FileParser::IsYouReadTheFileData()
{
	if (!m_fileData.empty())
		return true;
	else
	{
#ifdef _WIN32
		std::wcerr << L"Is you read the file data? Maybe there is no data here?" << std::endl;
#elif __linux__
		std::cerr << "Is you read the file data? Maybe there is no data here?" << std::endl;
#endif
		return false;
	}
};
std::wstring Files::FileParser::UTF8toUTF16(const std::string& element)
{
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(element);
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
#ifdef _WIN32
		std::wcout << L"File data:" << std::endl;
		for (auto& string : m_fileData)
			std::wcout << L'\t' << UTF8toUTF16(string) << std::endl;
#elif __linux__
		std::cout << "File data:" << std::endl;
		for (auto& string : m_fileData)
			std::cout << '\t' << string << std::endl;
#endif
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
void Files::FileParser::DeleteWords(const std::vector<std::string>& argv)
{
	String utf8str;
	std::size_t startPos = 0, i = 0, argvLength = 0;
	std::size_t argc = argv.size();
	if (IsYouReadTheFileData())
	{
		for (auto fileString = m_fileData.begin(); fileString != m_fileData.end(); ++fileString)
		{
			for (i = 0; i < argc; i++)
			{
#ifdef _WIN32
				// ввод с терминала Windows в кодировке Windows-1251 (CP1251)
				char str[100];
				wchar_t wstr[50];
				MultiByteToWideChar(1251, MB_PRECOMPOSED, argv[i].data(), -1, wstr, sizeof(wstr) / sizeof(wstr[0]));
				WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, sizeof(str) / sizeof(str[0]), 0, 0);
				utf8str = std::move(str);

				// не читает, получаемая строка остаётся неизменной (нулевой)
				//char* argv_ = argv[i];
				//std::size_t lenArgv_ = sizeof(argv[i]) / sizeof(argv[i][0]);
				//char argv__[50]{ "" };
				//char* a = argv__;
				//std::size_t lenA = sizeof(argv__) / sizeof(argv__[0]);
				//libiconv_t conv = libiconv_open("CP1251", "UTF-8");
				//std::wcout << libiconv(conv, &argv_, &lenArgv_, &a, &lenA) << std::endl;
				//libiconv_close(conv);
				//utf8str = a;
#elif __linux__
				// с Линукса же в UTF-8
				utf8str = argv[i];
#endif
				argvLength = utf8str.length();
				startPos = fileString->find(utf8str);
				if (startPos == String::npos)
					continue;
				else
					while (startPos != String::npos)
					{
						if ((startPos + argvLength) < fileString->length() &&
							std::isspace(fileString->at(startPos + argvLength)))
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
			if (fileString->find_first_not_of(" \t\n\v\f\r") == String::npos)
				fileString = m_fileData.erase(fileString);
			else
				++fileString;
	}
};
void Files::FileParser::SortFileStrings(bool aIsAscending)
{
	std::wstring element1UTF16;
	std::wstring element2UTF16;
	// нужно сбросить локаль, иначе начнут кидаться исключения, особенно приколько с Линуксом,
	// там при наличии в /etc/locale.gen нужной кодировки всё-равно её не видит
	std::locale loc = std::locale("");
	if (IsYouReadTheFileData())
	{
		auto toLower = [&](std::wstring& s, const std::locale& loc) {
			std::transform(s.begin(), s.end(), s.begin(),
				[&loc](wchar_t c) { return std::tolower(c, loc); });
			return s;
		};
		std::sort(m_fileData.begin(), m_fileData.end(), [&](auto& element1, auto& element2) {
			element1UTF16 = UTF8toUTF16(element1);
			element2UTF16 = UTF8toUTF16(element2);
			if (aIsAscending)
				return (toLower(element1UTF16, loc) < toLower(element2UTF16, loc));
			else
				return (toLower(element1UTF16, loc) > toLower(element2UTF16, loc));
			});
	}
};

