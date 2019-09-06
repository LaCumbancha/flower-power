#include <iostream>
#include <unistd.h>
#include "Scheduler.h"

pid_t Scheduler::newProcess(Job* job) {
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        job->run();
    }

    return pid;
}
