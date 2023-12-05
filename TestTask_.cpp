// TestTask.cpp: определяет точку входа для приложения.

#include "TestTask.h"

int main(int argc, char* argv[])
{	
	std::setlocale(LC_ALL, "ru_RU.UTF-8");

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
