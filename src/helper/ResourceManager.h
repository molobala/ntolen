#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <string>
#pragma once

class ResourceManager
{
public:
    ResourceManager();
    static std::unique_ptr<std::string> readSystemFile(const std::string& path);
    static std::string realPath(const std::string& path);
    static bool isDirectory(const std::string& path);
    static bool isRegularFile(const std::string& path);
private:

};

#endif