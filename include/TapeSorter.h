#pragma once
#include "FileTapeDevice.h"
#include <algorithm>
#include <limits>
#include <queue>

class TapeSorter 
{
    FileTapeDevice& inputTape;
    FileTapeDevice& outputTape;

    void mergeSort(std::vector<int>& data);
    void merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& result);
    void splitAndSortTempFiles(const std::string& inputPath, size_t blockSize);
    void mergeTempFiles(const std::string& outputPath);
    std::vector<std::string> tempFiles; // Хранение имен временных файлов
public:
    TapeSorter(FileTapeDevice& input, FileTapeDevice& output);
    void externalSort(const std::string& inputPath, const std::string& outputPath, size_t blockSize);
};
