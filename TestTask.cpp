// TestTask.cpp: определяет точку входа для приложения.
//

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _GLIBCXX_SANITIZE_VECTOR
//#define _GLIBCXX_USE_CXX11_ABI
//#define __GLIBCXX__

#include "TestTask.h"
#include <algorithm>
#include <vector>
#include <cstring>

#include <iostream>
#ifdef _WIN32
//#include <stdio.h>
//#include <io.h>
//#include <fcntl.h>
//#include <Windows.h>
#include <clocale>
#include <codecvt>
//std::string UTF8ToANSI(std::string s)
//{
//	BSTR    bstrWide;
//	char* pszAnsi;
//	int     nLength;
//	const char* pszCode = s.c_str();
//
//	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
//	bstrWide = SysAllocStringLen(NULL, nLength);
//
//	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);
//
//	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
//	pszAnsi = new char[nLength];
//
//	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
//	SysFreeString(bstrWide);
//
//	std::string r(pszAnsi);
//	delete[] pszAnsi;
//	return r;
//}
std::string utf8_to_string(const std::string& utf8str, const std::locale& loc)
{
	// UTF-8 to wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr = wconv.from_bytes(utf8str);
	// wstring to string
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}
#endif

int main(int argc, char* argv[])
{
//	using String = std::string;
//	using FileInputStream = std::ifstream;
//	using FileOutputStream = std::ofstream;
//
//	std::setlocale(LC_ALL, "russian");
////#ifdef _WIN32
////	//std::locale::global(std::locale(""));
////	//std::locale l_utf8(std::locale(), new std::codecvt_utf8<char, 0x10ffffUL, std::codecvt_mode::consume_header>);
////	//system("chcp 65001");
////#endif
//	String fileString;
//	FileInputStream fromFile("./TestFile.txt");
////#ifdef _WIN32
////	//fromFile.imbue(l_utf8);
////#endif
//	std::vector<String> fileData;
//	std::size_t startPos = 0, i = 0, argvLength = 0;
//	char* argv_i = nullptr;
//	//int argc = 2;
//	//char *argv[] = { "Type", "Кот" };
//	for (i = 0; i < argc; i++)
//		std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
//	if (fromFile.is_open())
//	{
//		while (std::getline(fromFile, fileString, '\n'))
//		{
//#ifdef _WIN32
//			fileString = utf8_to_string(fileString, std::locale(".1251"));
//#endif
//			if (argc > 1)
//			{
//				for (i = 0; i < argc; i++)
//				{
//					argv_i = argv[i];
//					argvLength = strnlen(argv_i, 100);
//					startPos = fileString.find(argv_i);
//					if (startPos == String::npos)
//						continue;
//					else
//						while (startPos != String::npos)
//						{
//							if (std::isspace(fileString.at(startPos + argvLength)))
//								fileString.erase(startPos, argvLength + 1);
//							else
//								fileString.erase(startPos, argvLength);
//							startPos = fileString.find(argv_i, startPos + argvLength);
//						};
//				}
//			}
//			if (fileString.find_first_not_of(' ') != std::string::npos)
//				fileData.push_back(fileString);
//		}
//	}
//	fromFile.close();
//	std::sort(fileData.begin(), fileData.end(), [](const auto& element1, const auto& element2) {
//		const auto result = std::mismatch(element1.cbegin(), element1.cend(), element2.cbegin(), element2.cend(),
//			[](const auto& char1, const auto& char2) {
//				bool isTrue = std::tolower(char1) == std::tolower(char2);
//				return isTrue;
//			});
//		return (result.second != element2.cend()) && (result.first == element1.cend() ||
//			std::tolower(*result.first) < std::tolower(*result.second));
//		});
//	//std::sort(fileData.begin(), fileData.end(), [](const std::string& str1, const std::string& str2) {
//	//	return std::lexicographical_compare(str1.begin(), str1.end(), str2.begin(), str2.end(),
//	//	[](const unsigned char& char1, const unsigned char& char2) {
//	//			return std::tolower(char1) < std::tolower(char2);
//	//		});
//	//	});
//	FileOutputStream inFile("./NewTestFile.txt", std::ios_base::trunc);
//#ifdef _WIN32
//	//inFile.imbue(l_utf8);
//#endif
//	//std::string ANSIstring;
//	for (const auto& string : fileData)
//	{
////#ifdef _WIN32
//		std::cout << string << std::endl;
//		inFile << string << '\n';
////#else
////		std::cout << string << std::endl;
////		inFile << string << '\n';
////#endif
//	}
//	inFile.close();
//
//	std::cout << "Выполнено!" << std::endl;
//
//	//std::system("pause");



	return 0;
}
