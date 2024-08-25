#include "ResourceManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager()
{
}

std::unique_ptr<std::string> ResourceManager::readSystemFile(const std::string &path)
{
    if (!std::filesystem::is_regular_file(path))
    {
        return std::unique_ptr<std::string>(nullptr);
    }
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return std::make_unique<std::string>(std::move(contents));
    }

    return std::unique_ptr<std::string>(nullptr);
}

std::string ResourceManager::realPath(const std::string &path)
{
    try
    {
        return std::filesystem::canonical(path);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}

bool ResourceManager::isDirectory(const std::string &path)
{

    return std::filesystem::is_directory(path);
}

bool ResourceManager::isRegularFile(const std::string &path)
{

    return std::filesystem::is_regular_file(path);
}