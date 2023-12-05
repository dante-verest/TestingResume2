// TestTask.cpp: определяет точку входа для приложения.

#include "TestTask.h"
#ifdef _WIN32
#pragma warning(disable: 6031)
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#endif

int main(int argc, char* argv[])
{	
	std::setlocale(LC_ALL, "ru_RU.UTF-8");
#ifdef _WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif

	std::unique_ptr<Files::FileParser> file = std::make_unique<Files::FileParser>();
	file->OpenFile("./TestFile.txt", std::ios_base::in);
	file->ReadFromFile();
	file->CloseFile();
	file->ShowFileData();

	const int argc_ = 2;
	const char *argv_[argc_] = { "Type", "Кот" };

	file->DeleteWords(argc_, argv_);
	file->DeleteEmptyStrings();
	file->SortFileStrings();
	file->ShowFileData();
	file->OpenFile("./NewTestFile.txt", std::ios_base::out | std::ios_base::trunc);
	file->WriteToFile();

	return 0;
}
