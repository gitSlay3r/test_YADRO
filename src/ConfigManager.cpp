#include "ConfigManager.h"

ConfigManager::ConfigManager(const std::string& configFilePath) 
{
    std::ifstream configFile(configFilePath);
    if (!configFile) 
    {
        std::cerr << "Unable to open config file: " << configFilePath << std::endl;
        // Установка значений по умолчанию
        config["readDelay"] = 0;
        config["writeDelay"] = 0;
        config["rewindDelay"] = 0;
        config["moveDelay"] = 0;
        return;
    }
    std::string line;
    while (std::getline(configFile, line)) 
    {
        auto delimiterPos = line.find('=');
        std::string key = line.substr(0, delimiterPos);
        int value = std::stoi(line.substr(delimiterPos + 1));
        config[key] = value;
    }
}

int ConfigManager::get(const std::string& key) const 
{
    auto it = config.find(key);
    if (it != config.end()) 
    {
        return it->second;
    }
    std::cerr << "Config key not found: " << key << std::endl;
    return -1;
}