// TestTask.cpp: определяет точку входа для приложения.
//

#include "TestTask.h"
#include <algorithm>
#include <vector>

int main(int argc, char* argv[])
{
	std::string fileString;
	std::ifstream fromFile("TestFile.txt");
	std::vector<std::string> fileData;
	std::size_t startPos = 0;
	if (fromFile.is_open())
	{
		while (std::getline(fromFile, fileString, '\n'))
		{
			if (argc > 1)
			{
				startPos = fileString.find(argv[1]);
				while (startPos != std::string::npos)
				{
					fileString.erase(startPos, strlen(argv[1]));
					startPos = fileString.find(argv[1], startPos + strlen(argv[1]));
				}
			}
			fileData.push_back(fileString);
		}
	}
	fromFile.close();
	std::sort(fileData.begin(), fileData.end());
	std::ofstream inFile("NewTestFile.txt");
	for (const std::string& string : fileData)
	{
		inFile << string << '\n';
	}
	inFile.close();

	std::system("pause");
	return 0;
}
