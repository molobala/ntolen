#include "Path.h"
#include <vector>

using namespace ntolen;


static bool isSeparator(char c)
{
    // Slash is a separator on POSIX and Windows.
    if (c == '/') return true;

    // Backslash is only a separator on Windows.
#ifdef _WIN32
    if (c == '\\') return true;
#endif

    return false;
}

#ifdef _WIN32
static bool isDriveLetter(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
#endif

// Gets the length of the prefix of [path] that defines its absolute root.
//
// Returns 1 the leading "/". On Windows, also handles drive letters ("C:" or
// "C:\").
//
// If the path is not absolute, returns 0.
static size_t absolutePrefixLength(const char* path)
{
#ifdef _WIN32
    // Drive letter.
  if (isDriveLetter(path[0]) && path[1] == ':')
  {
    if (isSeparator(path[2]))
    {
      // Fully absolute path.
      return 3;
    } else {
      // "Half-absolute" path like "C:", which is relative to the current
      // working directory on drive. It's absolute for our purposes.
      return 2;
    }
  }

  // TODO: UNC paths.

#endif

    // POSIX-style absolute path or absolute path in the current drive on Windows.
    if (isSeparator(path[0])) return 1;

    // Not absolute.
    return 0;
}

Path::Path(const char *d) : data(d) {}
Path::Path(const std::string& from) : data(from) {}

Path *Path::dirName()
{
    auto l = data.length();
    for (size_t i = l - 1; i < l; i--)
    {
        if (isSeparator(data[i]))
        {
            data = data.substr(0, i);
            return this;
        }
    }

    // If we got here, there was no separator so it must be a single component. data is an empty string now
    data.clear();
    return this;
    
}
Path *Path::removeExtension()
{
    auto l = data.length();
    for (size_t i = l - 1; i < l; i--)
    {
        // If we hit a path separator before finding the extension, then the last
        // component doesn't have one.
        if (isSeparator(data[i])) return this;

        if (data[i] == '.')
        {
            data = data.substr(0, i);
        }
    }
    return this;
}
Path *Path::join(const char *name)
{
    if (!data.empty() && !isSeparator(data.back()))
    {
        appendChar('/');
    }
    return appendString(name);
}
Path *Path::appendChar(char c)
{
    data.push_back(c);
    return this;
}
Path *Path::appendString(const std::string &str)
{
    data.append(str);
    return this;
}
typedef struct {
    std::string::iterator start;
    std::string::iterator end;
} Slice;

Path *Path::normalize()
{
     // Split the path into components.
    std::vector<Slice> components;


    std::string::iterator start = data.begin();
    std::string::iterator end = data.begin();

    // Split into parts and handle "." and "..".
    int leadingDoubles = 0;
    for (;;)
    {
        if (*end == '\0' || isSeparator(*end))
        {
            // Add the current component.
            if (start != end)
            {
                size_t length = end - start;
                if (length == 1 && start[0] == '.')
                {
                    // Skip "." components.
                }
                else if (length == 2 && start[0] == '.' && start[1] == '.')
                {
                    // Walk out of directories on "..".
                    if (components.size() > 0)
                    {
                        // Discard the previous component.
                        // numComponents--;
                        components.pop_back();
                    }
                    else
                    {
                        // Can't back out any further, so preserve the "..".
                        leadingDoubles++;
                    }
                }
                else
                {
                    // if (numComponents >= MAX_COMPONENTS)
                    // {
                    //     fprintf(stderr, "Path cannot have more than %d path components.\n",
                    //             MAX_COMPONENTS);
                    //     exit(1);
                    // }

                    components.push_back(std::move(Slice{start, end}));
                    // numComponents++;
                }
            }

            // Skip over separators.
            while (end[0] != '\0' && isSeparator(end[0])) end++;

            start = end;
            if (end[0] == '\0') break;
        }

        end++;
    }

    // Preserve the path type. We don't want to turn, say, "./foo" into "foo"
    // because that changes the semantics of how that path is handled when used
    // as an import string.
    bool needsSeparator = false;

    std::unique_ptr<Path> result = std::make_unique<Path>("");
    size_t prefixLength = absolutePrefixLength(data.c_str());
    if (prefixLength > 0)
    {
        // It's an absolute path, so preserve the absolute prefix.
        Slice slice;
        slice.start = data.begin();
        slice.end = data.begin() + prefixLength;
        result->appendString(std::string(slice.start, slice.end));
    }
    else if (leadingDoubles > 0)
    {
        // Add any leading "..".
        for (int i = 0; i < leadingDoubles; i++)
        {
            if (needsSeparator) {
                result->appendChar('/');
            }
            result->appendString("..");
            needsSeparator = true;
        }
    }
    else if (data[0] == '.' && isSeparator(data[1]))
    {
        // Preserve a leading "./", since we use that to distinguish relative from
        // logical imports.
        result->appendChar('.');
        needsSeparator = true;
    }

    for (auto it: components)
    {
        if (needsSeparator) result->appendChar('/');
        result->appendString(std::string(it.start, it.end));
        needsSeparator = true;
    }

    if (result->data.empty()) result->appendChar('.');

    // Copy back into the original path.
    this->data = std::move(result->data);
    return this;
}
const char *Path::toString() const
{
    return data.c_str();
}
PathType Path::type(const char *path)
{
     if (absolutePrefixLength(path) > 0) return PATH_TYPE_ABSOLUTE;
    // See if it must be relative.
    if ((path[0] == '.' && isSeparator(path[1])) ||
        (path[0] == '.' && path[1] == '.' && isSeparator(path[2])))
    {
        return PATH_TYPE_RELATIVE;
    }
    // Otherwise, we don't know.
    return PATH_TYPE_SIMPLE;
    
}