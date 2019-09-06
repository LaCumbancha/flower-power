#include "System.h"
#include "scheduler/Scheduler.h"

pid_t System::run() {
    pid_t pid;

    // Running producer process.
    pid = Scheduler::newProcess(&producerJob);

    // Running sale process (only if its parent process).
    if (pid != CHILD_PROCESS_PID) {
        pid = Scheduler::newProcess(&sellerJob);
    }

    return pid;
}

Config *System::getConfig() {
    return &_config;
}
