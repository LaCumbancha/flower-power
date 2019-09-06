#ifndef FLOWER_POWER_SCHEDULER_H
#define FLOWER_POWER_SCHEDULER_H
#define CHILD_PROCESS_PID 0


#include <iostream>
#include "../job/Job.h"

class Scheduler {

public:
    static pid_t newProcess(Job* job);
};


#endif //FLOWER_POWER_SCHEDULER_H
