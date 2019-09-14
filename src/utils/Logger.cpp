#include <sys/time.h>
#include "Logger.h"

Pipe* Logger::logPipe = new Pipe();
std::string Logger::logFile = "../logs/" + date("%Y%m%d") + ".log";

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

    exit(EXIT_SUCCESS);
}

void Logger::close() {
    Logger::logPipe->~Pipe();
}

void Logger::info(const std::string& text) {
    std::string log = mainLog();
    log += "[INFO] " + text + "|||";
    Logger::logPipe->write(log);
}

void Logger::warn(const std::string& text) {
    std::string log = mainLog();
    log += "[WARN] " + text + "|||";
    Logger::logPipe->write(log);
}

void Logger::error(const std::string& text) {
    std::string log = mainLog();
    log += "[ERROR] " + text + "|||";
    Logger::logPipe->write(log);
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

    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

    char currentTime[84] = "";
    sprintf(currentTime, "%s:%d", buffer, milli);
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
