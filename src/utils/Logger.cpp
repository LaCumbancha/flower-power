#include <sys/time.h>
#include <vector>
#include "Logger.h"

LogLevel Logger::level = DEBUG;
Pipe* Logger::logPipe = new Pipe();
std::string Logger::logFile = "./logs/" + date("%Y%m%d") + ".log";
std::map<LogLevel, std::string> Logger::levelsMap = Logger::createLevelsMap();

void Logger::writing() {
    Logger::logPipe->setWriteMode();
}

void Logger::run() {
    std::string data;
    int status;

    // Creating log file
    std::ofstream outfile;
    outfile.open(Logger::logFile, std::ios::app);

    Logger::logPipe->setReadMode();
    while(Logger::logPipe->read(data, &status)){
        if (status == EXIT_SUCCESS) {
            Logger::cleanLog(data);
            outfile << data << std::endl;
        }
    }

    outfile << mainLog() << "[INFO] Closing log." << std::endl;
    outfile.close();
    exit(EXIT_SUCCESS);
}

void Logger::close() {
    Logger::logPipe->~Pipe();
}

void Logger::debug(const std::string& text) {
    if (Logger::level <= DEBUG) {
        std::string log = mainLog();
        log += "[DEBUG] " + text + "|||";
        Logger::logPipe->write(log);
    }
}

void Logger::info(const std::string& text) {
    if (Logger::level <= INFO) {
        std::string log = mainLog();
        log += "[INFO] " + text + "|||";
        Logger::logPipe->write(log);
    }
}

void Logger::warn(const std::string& text) {
    if (Logger::level <= WARN) {
        std::string log = mainLog();
        log += "[WARN] " + text + "|||";
        Logger::logPipe->write(log);
    }
}

void Logger::error(const std::string& text) {
    if (Logger::level <= ERROR) {
        std::string log = mainLog();
        log += "[ERROR] " + text + "|||";
        Logger::logPipe->write(log);
    }
}

std::string Logger::date(const char* format) {
    time_t now = ::time(nullptr);
    struct tm tstruct{};
    char  buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), format, &tstruct);
    return buf;
}

std::string Logger::time() {
    timeval curTime{};
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    std::string milliText;
    if (milli >= 100) {
        milliText = std::to_string(milli);
    } else {
        if (milli >= 10) {
            milliText = "0" + std::to_string(milli);
        } else {
            milliText = "00" + std::to_string(milli);
        }
    }

    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

    char currentTime[84] = "";
    sprintf(currentTime, "%s.%s", buffer, milliText.c_str());
    return currentTime;
}

std::string Logger::mainLog() {
    std::string log;
    log += "[";
    log += time();
    log += "] [";
    log += std::to_string(getpid());
    log += "] ";
    return log;
}

void Logger::cleanLog(std::string& log) {
    size_t pos = log.find("|||");

    if (pos != std::string::npos) {
        // If found then erase it from string.
        log.erase(pos, log.length() - pos);
    }
}

std::string Logger::getLoggingLevel() {
    return Logger::levelsMap[Logger::level];
}

void Logger::setLoggingLevel(int newLevel) {
    Logger::level = LogLevel(newLevel);
}
