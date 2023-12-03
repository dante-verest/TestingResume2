#include "FileParser.h"
#include <iostream>
#include <algorithm>

Files::FileParser::FileParser() 
	: m_pFileName(nullptr)
{
	if (m_file.is_open())
		m_file.close();
};
Files::FileParser::FileParser(const char* aFileName,
	std::ios::openmode mode) 
	: m_pFileName(aFileName)
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
	//std::cout << std::setlocale(LC_ALL, "ru_RU.CP1251") << std::endl;
	//std::wcout << L"\xEF\xBB\xBF\xD0\xBF\xD1\x80\xD0\xBE\xD0\xB2\xD0\xB5\xD1\x80\xD0\xBA\xD0\xB0";
#ifdef _WIN32
	//std::cout << UTF8toANSI(std::string("Выполнено!"), std::locale("")) << std::endl;
	std::wcout << L"Выполнено!" << std::endl;
#elif __linux__
	std::cout << "Выполнено!" << std::endl;
#endif
	//wprintf("Выполнено!");
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
#ifdef _WIN32
Files::FileParser::String Files::FileParser::
ANSItoUTF8(const Files::FileParser::String& ansistr, const std::locale& loc)
{
	// ANSI to UTF-8
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring utf16str = std::wstring(ansistr.begin(), ansistr.end());
	return wconv.to_bytes(utf16str);
	// wstring to string
	//std::vector<char> buf(wstr.size());
	//std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	//return std::string(buf.data(), buf.size());
};
Files::FileParser::String Files::FileParser::
UTF8toANSI(const Files::FileParser::String& utf8str, const std::locale& loc)
{
	// UTF-8 to wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wConverter;
	std::wstring wstr = wConverter.from_bytes(utf8str);
	// wstring to string
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
};
#endif

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
		std::cout << "File data: " << std::endl;
		for (auto& string : m_fileData)
		{
#ifdef _WIN32
			//string = UTF8toANSI(string, std::locale(".1251"));
			//std::wcout << L'\t' << std::wstring(string.begin(), string.end()) << std::endl;
			std::cout << '\t' << string << std::endl;
#elif __linux__
			std::cout << '\t' << string << std::endl;
#endif
		}
		std::cout << std::endl;
	}
};
void Files::FileParser::Clear()
{
	if (IsYouReadTheFileData())
		m_fileData.clear();
};
void Files::FileParser::ReadFromFile()
{
	std::string fileString;
	if (IsYourFileOpen())
	{
		while (std::getline(m_file, fileString, '\n'))
		{
#ifdef _WIN32
			//m_fileData.push_back(ANSItoUTF8(fileString, std::locale("")));
#endif
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
	std::string utf8str;
	std::size_t startPos = 0, i = 0, argvLength = 0;
	if (IsYouReadTheFileData())
	{
		for (auto fileString = m_fileData.begin(); fileString != m_fileData.end(); ++fileString)
		{
			// предполагается, что файл в Linux и Windows записан в UTF-8
#ifdef _WIN32
			//*fileString = UTF8toString(fileString->data(), std::locale(".1251"));
#endif
			if (argc > 1)
			{
				for (i = 0; i < argc; i++)
				{
#ifdef _WIN32
					//char str[100];
					//wchar_t wstr[50];
					argvLength = strnlen(argv[i], 100);
					std::unique_ptr<char> str = std::make_unique<char>(argvLength);
					std::unique_ptr<wchar_t> wstr = std::make_unique<wchar_t>(argvLength);
					MultiByteToWideChar(1251, MB_PRECOMPOSED, argv[i], -1, wstr.get(), sizeof(argvLength) / sizeof(wstr.get()[0]));
					WideCharToMultiByte(CP_UTF8, 0, wstr.get(), -1, str.get(), sizeof(argvLength) / sizeof(str.get()[0]), 0, 0);
					//utf8str = UTF8toANSI(utf8str, std::locale(".1251"));
					std::cout << "\t\targv[i] = " << str << std::endl;
					utf8str = str.get();
					std::cout << "\t\tutf8str = " << utf8str << std::endl;
					//utf8str = ANSItoUTF8(utf8str, std::locale(""));
					//*fileString = ANSItoUTF8(fileString->data(), std::locale(""));
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
#ifdef _WIN32
	loc = std::locale("");
#elif __linux__
	//loc = std::locale("ru_RU");
#endif
	if (IsYouReadTheFileData())
	{
		std::sort(m_fileData.begin(), m_fileData.end(), [&](const auto& element1, const auto& element2) {
			const auto result = std::mismatch(element1.cbegin(), element1.cend(), element2.cbegin(), element2.cend(),
			[&](const auto& char1, const auto& char2) {
					bool isTrue = std::tolower(char1, loc) == std::tolower(char2, loc);
					return isTrue;
				});
		if (aIsAscending)
			return (result.second != element2.cend()) && (result.first == element1.cend() ||
				std::tolower(*result.first, loc) < std::tolower(*result.second, loc));
		else
			return (result.second != element2.cend()) && (result.first == element1.cend() ||
				std::tolower(*result.first, loc) > std::tolower(*result.second, loc));
			});
	}
};

