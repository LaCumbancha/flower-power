#ifndef FLOWER_POWER_LOGGER_H
#define FLOWER_POWER_LOGGER_H


#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <ctime>
#include <map>
#include <sys/time.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "Pipe.h"


enum LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {

public:
    explicit Logger() = default;

    static void run();
    static void close();
    static void writing();
    static void debug(const std::string& text);
    static void info(const std::string& text);
    static void warn(const std::string& text);
    static void error(const std::string& text);

    static std::string getLoggingLevel();
    static std::map<LogLevel, std::string> createLevelsMap() {
        std::map<LogLevel, std::string> map;
        map[DEBUG] = "DEBUG";
        map[INFO] = "INFO";
        map[WARN] = "WARN";
        map[ERROR] = "ERROR";
        return map;
    }

    static void setLoggingLevel(int newLevel);

private:
    static Pipe* logPipe;
    static std::string logFile;
    static std::string logFolder;

    static LogLevel level;
    static std::map<LogLevel, std::string> levelsMap;

    static std::string time();
    static std::string mainLog();
    static void cleanLog(std::string& log);
    static std::string date(const char* format = "%Y-%m-%d.%X");

    static void createLogsFolder();
};

#endif //FLOWER_POWER_LOGGER_H
