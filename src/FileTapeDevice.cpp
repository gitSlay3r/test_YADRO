#include "FileTapeDevice.h"

FileTapeDevice::FileTapeDevice(const std::string& fileName, bool isTemp)
    : fileName(fileName), isTemp(isTemp)
{
    // Путь к файлу конфигурации
    const std::string configFilePath = "config.txt";

    // Создание объекта ConfigManager и загрузка конфигурации
    ConfigManager config(configFilePath);

    // Инициализация задержек
    readDelay = config.get("readDelay");
    writeDelay = config.get("writeDelay");
    rewindDelay = config.get("rewindDelay");
    moveNextDelay = config.get("moveDelay");
    
    fileStream.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);

    if (!fileStream.is_open() && isTemp) 
    {
        fileStream.open(fileName, std::fstream::out | std::fstream::binary);
        fileStream.close();
        fileStream.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);
    }

    if (!fileStream.is_open()) 
    {
        throw std::runtime_error("Unable to open file: " + fileName);
    }
}

FileTapeDevice::~FileTapeDevice() 
{
    fileStream.close();
    if (isTemp) 
    {
        std::remove(fileName.c_str());
    }
}

bool FileTapeDevice::read(int& data) 
{
    if (!fileStream.read(reinterpret_cast<char*>(&data), sizeof(data))) 
    {
        return false;
    }
    applyDelay(readDelay);
    return true;
}

void FileTapeDevice::write(int data) {
    fileStream.write(reinterpret_cast<const char*>(&data), sizeof(data));
    if (!fileStream) 
    {
        throw std::runtime_error("Write operation failed on file: " + fileName);
    }
    applyDelay(writeDelay);
}

void FileTapeDevice::rewind() 
{
    fileStream.clear();
    fileStream.seekg(0, std::ios::beg);
    fileStream.seekp(0, std::ios::beg);
    applyDelay(rewindDelay);
}

void FileTapeDevice::moveNext() {
    fileStream.seekg(sizeof(int), std::ios::cur);
    fileStream.seekp(sizeof(int), std::ios::cur);
    applyDelay(moveNextDelay);
}

std::vector<std::unique_ptr<FileTapeDevice>> FileTapeDevice::createTempTapes(size_t count, const std::string& dir) 
{
    std::vector<std::unique_ptr<FileTapeDevice>> tempTapes;
    for (size_t i = 0; i < count; ++i) 
    {
        std::ostringstream tempFileName;
        tempFileName << dir << "/temp_tape_" << i << ".dat";
        tempTapes.push_back(std::make_unique<FileTapeDevice>(tempFileName.str(), true));
    }
    return tempTapes;
}
