#ifdef RUN_TESTS
#include "pch.h"
#include "gtest/gtest.h"
#include "TapeSorter.h"
#include "FileTapeDevice.h"

// ��������� � ���������� ������
#define LargeDataTest 

// ��������� ����� � N ���������� ������ �������
void generateRandomInputFile(const std::string& filePath, size_t count)
{
	std::ofstream file(filePath, std::ios::binary); 
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	int randomValue;

	for (size_t i = 0; i < count; ++i) 
	{
		randomValue = std::rand();
		file.write(reinterpret_cast<char*>(&randomValue), sizeof(randomValue));
	}

	file.close();
}

// ��������, ��� ���� �������� ����� � ������� �����������
bool isFileSorted(const std::string& filePath) 
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file) 
	{
		// �� ������� ������� ����
		return false;
	}

	int prevValue = std::numeric_limits<int>::min();
	int currentValue;

	while (file.read(reinterpret_cast<char*>(&currentValue), sizeof(currentValue))) 
	{
		if (currentValue < prevValue) 
		{
			return false; // ������� �����, ������� ������ �����������, ���� �� ������������
		}
		prevValue = currentValue;
	}

	// ��������� ����� ����� ��� ��������� �������, ���� ������������
	return true;
}

void createEmptyFile(const std::string& filePath) {
	// ��������� ���� ��� ������ (� ������� ���, ���� �� ����������)
	std::ofstream outputFile(filePath);

	// ���������, ������� �� ������ ����
	if (!outputFile.is_open())
	{
		std::cerr << "�� ������� �������/������� ����: " << filePath << "\n";
		return;
	}

	// ��������� ���� ����� ��������
	outputFile.close();
}

void readAndProcessFileBinary(const std::string& inputPath) 
{
	std::ifstream inputFile(inputPath, std::ios::binary);
	int value;

	while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(value))) 
	{
		// ��������� �������� 'value'
		std::cout << value << std::endl;
	}

	inputFile.close();
}

// ���� ���������� �������� ������ ������
TEST(TapeSorterTest, LargeDataTest)
{
	std::string inputFilePath = "test_files/large_input_data.dat";
	size_t dataSize = 1000000;  //������ ������
	generateRandomInputFile(inputFilePath, dataSize);
	
	// �������� ��������� �����
	std::string outputFilePath = "test_files/large_output_data.dat";
	createEmptyFile(outputFilePath);

	FileTapeDevice inputFile(inputFilePath, false);
	FileTapeDevice outputFile(outputFilePath, false);
	TapeSorter sorter(inputFile, outputFile); 
	size_t block_processing = 100000;
	sorter.externalSort(inputFilePath, outputFilePath, block_processing);
	// ��������, ��� ������ �������������
	ASSERT_TRUE(isFileSorted(outputFilePath));
	//readAndProcessFileBinary(outputFilePath);
}

int main(int argc, char** argv) 
{
	setlocale(LC_ALL, "Russian");
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif