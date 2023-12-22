#include "TapeSorter.h"

TapeSorter::TapeSorter(FileTapeDevice& input, FileTapeDevice& output)
    : inputTape(input), outputTape(output) 
{}

void TapeSorter::externalSort(const std::string& inputPath, const std::string& outputPath, size_t blockSize) 
{
    splitAndSortTempFiles(inputPath, blockSize);
    mergeTempFiles(outputPath);

    // Удаление временных файлов
    for (const auto& tempFile : tempFiles) 
    {
        std::remove(tempFile.c_str());
    }
}

void TapeSorter::splitAndSortTempFiles(const std::string& inputPath, size_t blockSize) 
{
    std::ifstream inputFile(inputPath, std::ios::binary);
    std::vector<int> buffer(blockSize);
    int value = 0;

    while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(value))) 
    {
        buffer.clear();
        buffer.push_back(value);
        size_t numValues = 1;

        // Чтение и сортировка данных в памяти до достижения ограничения blockSize
        while (numValues < blockSize && inputFile.read(reinterpret_cast<char*>(&value), sizeof(value))) 
        {
            buffer.push_back(value);
            numValues++;
        }

        mergeSort(buffer);

        // Запись отсортированных данных во временный файл
        std::string tempFilePath = "tmp/temp_file_" + std::to_string(tempFiles.size()) + ".dat";
        std::ofstream tempFile(tempFilePath, std::ios::binary);

        for (int num : buffer) 
        {
            tempFile.write(reinterpret_cast<char*>(&num), sizeof(num));
        }

        tempFiles.push_back(tempFilePath);
    }
}

// Структура для хранения значения и индекса потока, из которого оно было прочитано
struct StreamValue 
{
    int value;
    size_t index;

    StreamValue(int v, size_t i) : value(v), index(i) {}
};

// Компаратор для минимальной кучи, сравнивает значения
struct Compare 
{
    bool operator()(const StreamValue& a, const StreamValue& b) 
    {
        return a.value > b.value; // Минимальное значение должно быть сверху
    }
};

void TapeSorter::mergeTempFiles(const std::string& outputPath) 
{
    std::ofstream outputFile(outputPath, std::ios::binary);
    std::vector<std::ifstream> inputStreams(tempFiles.size());
    std::priority_queue<StreamValue, std::vector<StreamValue>, Compare> minHeap;

    // Открытие всех временных файлов для чтения
    for (size_t i = 0; i < tempFiles.size(); ++i) 
    {
        inputStreams[i].open(tempFiles[i], std::ios::binary);
        if (!inputStreams[i]) 
        {
            continue;
        }

        int value;
        if (inputStreams[i].read(reinterpret_cast<char*>(&value), sizeof(value)))
        {
            minHeap.push(StreamValue(value, i));
        }
    }

    // Слияние
    while (!minHeap.empty()) 
    {
        StreamValue current = minHeap.top();
        minHeap.pop();

        // Запись минимального значения в выходной файл
        outputFile.write(reinterpret_cast<char*>(&current.value), sizeof(current.value));

        // Чтение следующего значения из того же потока
        if (inputStreams[current.index].read(reinterpret_cast<char*>(&current.value), sizeof(current.value))) 
        {
            minHeap.push(StreamValue(current.value, current.index));
        }
    }

    // Закрытие файлов
    outputFile.close();
    for (size_t i = 0; i < inputStreams.size(); ++i) 
    {
        inputStreams[i].close();
    }
}


void TapeSorter::mergeSort(std::vector<int>& data)
{
    if (data.size() <= 1)
    {
        return;
    }

    const auto middle = data.begin() + data.size() / 2;
    std::vector<int> left(data.begin(), middle);
    std::vector<int> right(middle, data.end());

    mergeSort(left);
    mergeSort(right);

    merge(left, right, data);
}

void TapeSorter::merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& result)
{
    size_t i = 0, j = 0, k = 0;

    while (i < left.size() && j < right.size())
    {
        if (left[i] < right[j])
        {
            result[k++] = left[i++];
        }
        else
        {
            result[k++] = right[j++];
        }
    }

    while (i < left.size())
    {
        result[k++] = left[i++];
    }

    while (j < right.size())
    {
        result[k++] = right[j++];
    }
}
