#include "logger.h"

#include <SDL3/SDL.h>

using namespace ntolen;

void Logger::info(const char *format...)
{
    va_list args;
    va_start(args, format);
    // Green for info messages
    const char *color = "\033[32m"; // Green
    const char *reset = "\033[0m";  // Reset color

    char prefixed_format[256];
    snprintf(prefixed_format, sizeof(prefixed_format), "%s%s%s", color, format, reset);

    // Forward to SDL_LogMessageV
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, prefixed_format, args);

    va_end(args);
}
void Logger::debug(const char *format, ...)
{
    #ifdef Debug
    va_list args;
    va_start(args, format);

    // Forward to SDL_LogMessageV
    // Blue for debug messages
    const char *color = "\033[34m"; // Blue
    const char *reset = "\033[0m";  // Reset color

    char prefixed_format[256];
    snprintf(prefixed_format, sizeof(prefixed_format), "%s%s%s", color, format, reset);


    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, prefixed_format, args);

    va_end(args);
    #endif
}

void Logger::error(const char *format...)
{
    va_list args;
    va_start(args, format);
    // Red for error messages
    const char *color = "\033[31m"; // Red
    const char *reset = "\033[0m";  // Reset color

    char prefixed_format[256];
    snprintf(prefixed_format, sizeof(prefixed_format), "%s%s%s", color, format, reset);

    // Forward to SDL_LogMessageV
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, prefixed_format, args);

    va_end(args);
}