// TestTask.cpp: определяет точку входа для приложения.

#include "TestTask.h"
#include <filesystem>
#include <stdexcept>
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

	std::vector<std::string> commandParameters;
	commandParameters.reserve(argc);
	std::string inputFileName, outputFileName;
	if (argc > 1)
	{
		for (std::size_t i = 1; i < argc; i++)
			commandParameters.push_back(argv[i]);
		for (const auto& parameter : commandParameters)
			if (parameter == "--help")
			{
#ifdef _WIN32
				std::wcout << L"Два последних параметра слева-напрваво - пути читаемого и записываемого"
					"файлов. Сразу после TestTask идут слова, которые нужно обработать в читаемом файле." << std::endl;
#elif __linux__
				std::cout << "Два последних параметра слева-напрваво - пути читаемого и записываемого"
					"файлов. Сразу после TestTask идут слова, которые нужно обработать в читаемом файле." << std::endl;
#endif
				return 0;
			}
		// обязательно нужно ввести входной и выходной пути
		if (argc > 3 && std::filesystem::exists(commandParameters.at(argc - 3)))
		{
			inputFileName = std::move(commandParameters.at(argc - 3));
			outputFileName = std::move(commandParameters.at(argc - 2));
			commandParameters.erase(commandParameters.end() - 2, commandParameters.end());
		}
		else
			throw std::runtime_error(commandParameters.at(argc - 3) + ": No such file or incorrect unput parameters!");
	}

	file->OpenFile(inputFileName.c_str(), std::ios_base::in);
	file->ReadFromFile();
	file->CloseFile();
	file->ShowFileData();
	file->DeleteWords(commandParameters);
	file->DeleteEmptyStrings();
	file->SortFileStrings();
#ifdef _WIN32
	std::wcout << L"\nAfter deleting words and empty strings and sorting strings ascending order:\n";
#elif __linux__
	std::cout << "\nAfter deleting words and empty strings and sorting strings ascending order:\n";
#endif
	file->ShowFileData();
	file->OpenFile(outputFileName.c_str(), std::ios_base::out | std::ios_base::trunc);
	file->WriteToFile();

	return 0;
}
