#ifndef FLOWER_POWER_LOGGER_H
#define FLOWER_POWER_LOGGER_H


#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <ctime>
#include "Pipe.h"

class Logger {

public:
    explicit Logger() = default;

    static void run();
    static void close();
    static void writing();
    static void info(const std::string& text);
    static void warn(const std::string& text);
    static void error(const std::string& text);

private:
    static Pipe* logPipe;
    static std::string logFile;

    static std::string mainLog();
    static std::string time(const char* format = "%Y-%m-%d.%X");

};


#endif //FLOWER_POWER_LOGGER_H
