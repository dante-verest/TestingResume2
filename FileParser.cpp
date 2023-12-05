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
			//std::setlocale(LC_ALL, "en_US.UTF-16");
			//std::wcout << element1 << std::endl;
			//std::wcout << element2 << std::endl;
			//wprintf(element1.c_str());
				element1UTF16 = UTF8toUTF16(element1);
				element2UTF16 = UTF8toUTF16(element2);
			if (aIsAscending)
			{
				//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

				//std::wstring el1 = converter.from_bytes(element1);
				//std::wstring el2 = converter.from_bytes(element2);;


				//bool b = toLower(UTF8toUTF16(element1), loc) < toLower(UTF8toUTF16(element1), loc);
				//std::wcout << L'\t' << std::boolalpha << b << std::endl;
				//bool b = toLower(std::move(el1), loc) < toLower(std::move(el2), loc);
				return toLower(element1UTF16, loc) < toLower(element2UTF16, loc);
			}
			else
				return toLower(element1UTF16, loc) > toLower(element2UTF16, loc);
			});
		//std::sort(m_fileData.begin(), m_fileData.end(), [&](auto& element1, auto& element2) {
		//		FromChar* element1charBefore = element1.data();
		//		std::size_t element1charLengthBefore = strlen(element1charBefore);
		//		//std::unique_ptr<ToChar> element1charAfter = std::make_unique<ToChar>(element1charLengthBefore);
		//		//ToChar* el1 = std::move(element1charAfter.get());
		//		//std::size_t element1charLengthAfter = sizeof(el1) / sizeof(el1[0]);
		//		ToChar* el1 = new ToChar[element1charLengthBefore];
		//		std::size_t element1charLengthAfter = element1charLengthBefore;
		//		libiconv(conv, &element1charBefore, &element1charLengthBefore,
		//			&el1, &element1charLengthAfter);
		//		FromChar* element2charBefore = element2.data();
		//		std::size_t element2charLengthBefore = strlen(element2charBefore);
		//		std::unique_ptr<ToChar> element2charAfter = std::make_unique<ToChar>(element2charLengthBefore);
		//		ToChar* el2 = std::move(element2charAfter.get());
		//		//std::size_t element2charLengthAfter = sizeof(el2) / sizeof(el2[0]);
		//		//ToChar* el2 = new ToChar[element2charLengthBefore];
		//		std::size_t element2charLengthAfter = element2charLengthBefore;
		//		libiconv(conv, &element2charBefore, &element2charLengthBefore,
		//			&el2, &element2charLengthAfter);
		//		std::cout << toLower(el1, loc) << std::endl;
		//		std::cout << toLower(el2, loc) << std::endl;
		//	if (aIsAscending)
		//		return (toLower(el1, loc) < toLower(el2, loc));
		//	else
		//		return (toLower(el1, loc) > toLower(el2, loc));
		//});
		//libiconv_close(conv);
	}
};

