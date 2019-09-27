#include "ProcessKiller.h"

Pipe *ProcessKiller::_pidsPipe = new Pipe();
std::vector<pid_t> ProcessKiller::_pids = std::vector<pid_t>();

void ProcessKiller::run() {
    std::string data;
    int status;

    // Reading incoming stats.
    ProcessKiller::_pidsPipe->setReadMode();
    while (ProcessKiller::_pidsPipe->read(data, &status)) {
        if (status == EXIT_SUCCESS) {
            if (isAddIncoming(data)) {
                ProcessKiller::_pids.push_back(std::stoi(data.substr(2, data.size())));
                Logger::debug("Process " + data.substr(2, data.size()) + " added to Process Killer list.");
            } else if (isRemoveIncoming(data)) {
                removePidFromVector(std::stoi(data.substr(2, data.size())));
                Logger::debug("Process " + data.substr(2, data.size()) + " removed from Process Killer list.");
            } else if (isKillIncoming(data)) {
                killPIDs();
            }
        }
    }

    Logger::info("Closing Process Killer.");
    ProcessKiller::close();
    exit(EXIT_SUCCESS);
}

void ProcessKiller::close() {
    ProcessKiller::_pidsPipe->~Pipe();
}

void ProcessKiller::addingMode() {
    ProcessKiller::_pidsPipe->setWriteMode();
}

void ProcessKiller::addPID(pid_t pid) {
    ProcessKiller::_pidsPipe->write("A|" + std::to_string(pid));
}

void ProcessKiller::removePID(pid_t pid) {
    ProcessKiller::_pidsPipe->write("R|" + std::to_string(pid));
}

void ProcessKiller::killAll() {
    ProcessKiller::_pidsPipe->write("K|");
}

bool ProcessKiller::isAddIncoming(const std::string &data) {
    return data.substr(0, 2) == "A|";
}

bool ProcessKiller::isRemoveIncoming(const std::string &data) {
    return data.substr(0, 2) == "R|";
}

bool ProcessKiller::isKillIncoming(const std::string &data) {
    return data.substr(0, 2) == "K|";
}

void ProcessKiller::killPIDs() {
    int result;
    for (auto pid : ProcessKiller::_pids) {
        result = kill(pid, SIGTERM);
        Logger::debug("Killing process " + std::to_string(pid) + ". Result: " + std::to_string(result) + ".");
    }

    ProcessKiller::_pids.clear();
}

void ProcessKiller::removePidFromVector(int pid) {
    Logger::warn("List size before removing PID #" + std::to_string(pid) + ": " + std::to_string(ProcessKiller::_pids.size()));
    ProcessKiller::_pids.erase(std::remove(ProcessKiller::_pids.begin(), ProcessKiller::_pids.end(), pid),
                               ProcessKiller::_pids.end());
    Logger::warn("List size after removing PID #" + std::to_string(pid) + ": " + std::to_string(ProcessKiller::_pids.size()));
}
