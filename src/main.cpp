#ifdef RUN_MAIN
//#include "pch.h"
#include "TapeSorter.h"
#include "FileTapeDevice.h"

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
		return 1;
	}

	std::string inputFilePath = argv[1];
	std::string outputFilePath = argv[2];
	size_t block_processing = 10000;

	FileTapeDevice inputDevice(inputFilePath, false);
	FileTapeDevice outputDevice(outputFilePath, false);

	TapeSorter sorter(inputDevice, outputDevice);
	sorter.externalSort(inputFilePath, outputFilePath, block_processing);

	std::cout << "Sorting completed. Output is in: " << outputFilePath << std::endl;

	return 0;
}

#endif