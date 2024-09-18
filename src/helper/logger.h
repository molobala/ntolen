#ifndef LOGGER_H
#define LOGGER_H


namespace ntolen
{
    class Logger {
        public: 
        static void info(const char *format,...);
        static void debug(const char *format,...);
        static void error(const char *format,...);
    };

}
#endif