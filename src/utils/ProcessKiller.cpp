#include "ProcessKiller.h"

Pipe* ProcessKiller::_pidsPipe = new Pipe();
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
            } else if (isKillIncoming(data)) {
                killPIDs();
            } else if (isQuitIncoming(data)) {
                break;
            }
        }
    }

    Logger::info("Closing Process Killer.");
    ProcessKiller::_pidsPipe->~Pipe();
    exit(EXIT_SUCCESS);
}

void ProcessKiller::close() {
    ProcessKiller::_pidsPipe->write("QUIT");
    ProcessKiller::_pidsPipe->~Pipe();
}

bool ProcessKiller::isQuitIncoming(const std::string &data) {
    return data == "QUIT";
}

void ProcessKiller::addingMode() {
    ProcessKiller::_pidsPipe->setWriteMode();
}

void ProcessKiller::addPID(pid_t pid) {
    ProcessKiller::_pidsPipe->write("A|" + std::to_string(pid));
}

void ProcessKiller::killAll() {
    ProcessKiller::_pidsPipe->write("K|");
}

bool ProcessKiller::isAddIncoming(const std::string& data) {
    return data.substr(0, 2) == "A|";
}

bool ProcessKiller::isKillIncoming(const std::string& data) {
    return data.substr(0, 2) == "K|";
}

void ProcessKiller::killPIDs() {
    int result;
    for (auto pid : ProcessKiller::_pids) {
        result = kill(pid, SIGTERM);
        Logger::debug("Killing process " + std::to_string(pid) + ". Result: " + std::to_string(result) + ".");
    }
}
