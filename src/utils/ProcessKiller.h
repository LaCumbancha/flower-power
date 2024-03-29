#ifndef FLOWER_POWER_PROCESSKILLER_H
#define FLOWER_POWER_PROCESSKILLER_H


#include <vector>
#include <string>
#include <csignal>
#include <algorithm>
#include <bits/signum.h>
#include "Pipe.h"
#include "Logger.h"

class ProcessKiller {

public:
    explicit ProcessKiller() = default;

    static void run();
    static void close();
    static void finish();

    static void addingMode();
    static void removePID(int pid);
    static void addPID(pid_t pid);
    static void killAll();

private:
    static Pipe* _pidsPipe;
    static std::vector<pid_t> _pids;

    static bool isAddIncoming(const std::string& data);
    static bool isKillIncoming(const std::string& data);
    static bool isRemoveIncoming(const std::string& data);
    static bool isQuitIncoming(std::string &data);
    static void killPIDs();

    static void removePidFromVector(int pid);
};


#endif //FLOWER_POWER_PROCESSKILLER_H
