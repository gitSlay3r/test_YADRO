#ifdef RUN_TESTS
#include "pch.h"
#include "gtest/gtest.h"
#include "TapeSorter.h"
#include "FileTapeDevice.h"

// Включение и отключение тестов
#define LargeDataTest 

// Генерация файла с N случайными целыми числами
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

// Проверка, что файл содержит числа в порядке возрастания
bool isFileSorted(const std::string& filePath) 
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file) 
	{
		// Не удалось открыть файл
		return false;
	}

	int prevValue = std::numeric_limits<int>::min();
	int currentValue;

	while (file.read(reinterpret_cast<char*>(&currentValue), sizeof(currentValue))) 
	{
		if (currentValue < prevValue) 
		{
			return false; // Найдено число, которое меньше предыдущего, файл не отсортирован
		}
		prevValue = currentValue;
	}

	// Достигнут конец файла без нарушений порядка, файл отсортирован
	return true;
}

void createEmptyFile(const std::string& filePath) {
	// Открываем файл для записи (и создаем его, если не существует)
	std::ofstream outputFile(filePath);

	// Проверяем, успешно ли открыт файл
	if (!outputFile.is_open())
	{
		std::cerr << "Не удалось открыть/создать файл: " << filePath << "\n";
		return;
	}

	// Закрываем файл после создания
	outputFile.close();
}

void readAndProcessFileBinary(const std::string& inputPath) 
{
	std::ifstream inputFile(inputPath, std::ios::binary);
	int value;

	while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(value))) 
	{
		// Обработка значения 'value'
		std::cout << value << std::endl;
	}

	inputFile.close();
}

// Тест сортировки большого объема данных
TEST(TapeSorterTest, LargeDataTest)
{
	std::string inputFilePath = "test_files/large_input_data.dat";
	size_t dataSize = 1000000;  //размер данных
	generateRandomInputFile(inputFilePath, dataSize);
	
	// Создание выходного файла
	std::string outputFilePath = "test_files/large_output_data.dat";
	createEmptyFile(outputFilePath);

	FileTapeDevice inputFile(inputFilePath, false);
	FileTapeDevice outputFile(outputFilePath, false);
	TapeSorter sorter(inputFile, outputFile); 
	size_t block_processing = 100000;
	sorter.externalSort(inputFilePath, outputFilePath, block_processing);
	// Проверка, что данные отсортированы
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