#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

class ConfigManager 
{
private:
    std::unordered_map<std::string, int> config; // �������� ������������

public:
    ConfigManager(const std::string& configFilePath); // �����������
    int get(const std::string& key) const; 
    // ������� ��� ��������� 
    // �������� �� �����(��������� ��������) 
};