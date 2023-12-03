// TestTask.cpp: определяет точку входа для приложения.
//

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _GLIBCXX_SANITIZE_VECTOR
//#define _GLIBCXX_USE_CXX11_ABI
//#define __GLIBCXX__

#include "TestTask.h"

int main(int argc, char* argv[])
{	
	std::setlocale(LC_ALL, "ru_RU.UTF8");
	//std::setlocale(LC_ALL, "ru_RU.UTF8");
	//std::cout << std::setlocale(LC_ALL, NULL) << std::endl;
	
	//std::locale current_locale("");
	//std::locale::global(current_locale);

//#ifdef _WIN32
//	system("chcp 1251");
//#else
	//std::system("chcp 65001");
//#endif

	std::unique_ptr<Files::FileParser> file = std::make_unique<Files::FileParser>();
	file->OpenFile("./TestFile.txt", std::ios_base::in);
	file->ReadFromFile();
	file->CloseFile();
	file->ShowFileData();
	//int localArgc = argc - 1;
	////std::unique_ptr<std::unique_ptr<char>[]> localArgv = std::make_unique<char[]>(localArgc);
	//char** localArgv = new char*[localArgc];
	//for (std::size_t i = 0; i < argc; i++)
	//{
	//	//localArgv[i].reset(argv[i + 1]);
	//	localArgv = new char(argv[i + 1]);
	//}

	const int argc_ = 2;
	const char *argv_[argc_] = { "Type", "Кот" };

	//argc = 2;
	//argv[0] = std::move(argv_[0]);

	file->DeleteWords(argc_, argv_);
	file->DeleteEmptyStrings();
	file->SortFileStrings();
	file->ShowFileData();
	file->OpenFile("./NewTestFile.txt", std::ios_base::out | std::ios_base::trunc);
	file->WriteToFile();

	return 0;
}
