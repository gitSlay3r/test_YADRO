#pragma once
#include "TapeDevice.h"
#include <fstream>
#include "ConfigManager.h"
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <sstream>

class FileTapeDevice : public TapeDevice 
{
    std::fstream fileStream;
    std::string fileName;
    bool isTemp;
    int readDelay;
    int writeDelay;
    int rewindDelay;
    int moveNextDelay;

    void applyDelay(int milliseconds) const 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

public:
    FileTapeDevice(const std::string& fileName, bool isTemp = false);
    ~FileTapeDevice();

    FileTapeDevice(const FileTapeDevice&) = delete;
    FileTapeDevice& operator=(const FileTapeDevice&) = delete;

    bool read(int& data) override;  
    void write(int data) override;
    void rewind() override;
    void moveNext() override;

    static std::vector<std::unique_ptr<FileTapeDevice>> createTempTapes(size_t count, const std::string& dir);
};
