#include "FileParser.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "iconv.h"
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
std::wstring Files::FileParser::UTF8toUTF16(const FileParser::String& element)
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
void Files::FileParser::DeleteWords(int argc, const char** argv)
{
	String utf8str;
	std::size_t startPos = 0, i = 0, argvLength = 0;
	if (IsYouReadTheFileData())
	{
		for (auto fileString = m_fileData.begin(); fileString != m_fileData.end(); ++fileString)
		{
			if(argc > 1)
				for (i = 0; i < argc; i++)
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
					argvLength = utf8str.size();
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
			if (fileString->find_first_not_of(" \t\n\v\f\r") == String::npos)
				fileString = m_fileData.erase(fileString);
			else
				++fileString;
	}
};
void Files::FileParser::SortFileStrings(bool aIsAscending)
{
	//using FromChar = char;
	//using ToChar = char;
	std::wstring element1UTF16;
	std::wstring element2UTF16;
	std::locale loc;
	if (IsYouReadTheFileData())
	{
		//libiconv_t conv;
#ifdef _WIN32
		//conv = libiconv_open("UTF-8", "CP1251");
		loc = std::locale("");
#elif __linux__
		//conv = libiconv_open("UTF-8", "UTF-16");
		loc = std::locale("");
#endif
		auto toLower = [&](std::wstring& s, const std::locale& loc) {
			std::transform(s.begin(), s.end(), s.begin(),
				[&loc](wchar_t c) { return std::tolower(c, loc); });
			return s;
		};
		//auto test = std::vector<std::wstring>{
		//	L"Кот был красив. Кот был яркий, как огонь, очень рыжий, даже оранжевый.",
		//	L"У кота было белое брюшко.Кот крался за синичкой.Синичка, ничего не",
		//	L"подозревая, скакала по ступеньке.Синичке было весело в тот весенний",
		//	L"денёк.Синичка радостно тренькала.Кот напрягся для прыжка.Кот прыгнул,",
		//	L"кот промахнулся.",
		//	L" ",
		//	L"",
		//	L"your Type",
		//	L"Type or paste your content here Type or paste your content here Type",
		//	L"or paste your content here Type or paste your content here Type or paste your content here"
		//};
		std::sort(m_fileData.begin(), m_fileData.end(), [&](auto& element1, auto& element2) {
   char* element1charBefore = element1.data();
   std::size_t element1charBeforeLength = element1.length;
   std::unique_ptr<char> element1charAfter =
std::make_unique<char>(new char[element1charBeforeLength + 1]);
   icons(conv, &element1charBefore, &
element1charBeforeLength,&element1charAfter, &(
element1charBeforeLength + 1));
   
   char* element2charBefore = element2.data();
   std::size_t element2charBeforeLength = element1.length;
   std::unique_ptr<char> element2charAfter =
std::make_unique<char>(new char[element2charBeforeLength + 1]);
   icons(conv, &element2charBefore, &
element2charBeforeLength,&element2charAfter, &(
element2charBeforeLength + 1));

if (aIsAscending)
				return (toLower(element1charAfter, loc) < toLower(element2charAfter, loc));
			else
				return (toLower(element1, loc) > toLower(element2, loc));
		});
		iconv_close(conv);
	};
};

