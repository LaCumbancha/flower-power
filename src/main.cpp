#include "menu/MainMenu.h"
#include "utils/StatsCenter.h"
#include "utils/ContextStatus.h"

using namespace std;

int main(int argc, char *argv[]) {

    // Creating listener Logger.
    pid_t pid = fork();
    if (pid == CHILD_PROCESS_PID) {
        Logger::run();
    }

    Logger::writing();
    Logger::info("Initialized Logger in process with PID #" + std::to_string(pid) + ".");

    // Creating Stats Center.
    pid = fork();
    if (pid == CHILD_PROCESS_PID) {
        StatsCenter::run();
    }

    StatsCenter::addingMode();
    Logger::info("Initialized Stats Center in process with PID #" + std::to_string(pid) + ".");

    // Creating Context Status.
    pid = fork();
    if (pid == CHILD_PROCESS_PID) {
        ContextStatus::run();
    }

    ContextStatus::saveMode();
    Logger::info("Initialized Context Status in process with PID #" + std::to_string(pid) + ".");

    // Creating Process Killer.
    pid = fork();
    if (pid == CHILD_PROCESS_PID) {
        ProcessKiller::run();
    }

    ProcessKiller::addingMode();
    Logger::info("Initialized Process Killer in process with PID #" + std::to_string(pid) + ".");

    MainMenu menu = MainMenu();
    menu.show();

    ProcessKiller::close();
    ContextStatus::close();
    StatsCenter::close();

    Logger::info("Main program finished.");
    Logger::close();

    // TODO: Check exit codes.
    wait(nullptr);

    return EXIT_SUCCESS;
}
