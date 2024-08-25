#ifndef PATH_H
#define PATH_H

#include <string>

#pragma once

namespace ntolen
{
    typedef enum
    {
        // An absolute path, starting with "/" on POSIX systems, a drive letter on
        // Windows, etc.
        PATH_TYPE_ABSOLUTE,

        // An explicitly relative path, starting with "./" or "../".
        PATH_TYPE_RELATIVE,

        // A path that has no leading prefix, like "foo/bar".
        PATH_TYPE_SIMPLE,
    } PathType;
    class Path
    {
    public:
        Path(const char* d);
        Path(const std::string& from);
        Path* dirName();
        Path* removeExtension();
        Path* join(const char* name);
        Path* appendChar(char c);
        Path* appendString(const std::string& str);
        Path* normalize();
        const char* toString() const ;
        static PathType type(const char* path);
        bool operator ==(const Path& path) {
            return this->data == path.data;
        }

    private:
        std::string data;
    };

}

#endif