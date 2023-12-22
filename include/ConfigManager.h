#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

class ConfigManager 
{
private:
    std::unordered_map<std::string, int> config; // Хранение конфигураций

public:
    ConfigManager(const std::string& configFilePath); // Конструктор
    int get(const std::string& key) const; 
    // Функция для получения 
    // значения по ключу(различные задержки) 
};