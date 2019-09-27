#ifndef FLOWER_POWER_CLOSER_H
#define FLOWER_POWER_CLOSER_H


#include "StatsCenter.h"
#include "ContextStatus.h"
#include "ProcessKiller.h"

class Closer {

public:
    static void finishAuxJobs(int pid) {
        ProcessKiller::close();
        ContextStatus::close();
        StatsCenter::close();
        Logger::debug("Finishing auxiliary jobs for PID #" + std::to_string(pid) + ".");
        Logger::close();

        for (int fileDescriptor = getdtablesize(); fileDescriptor > 3;) close(--fileDescriptor);
    }

};


#endif //FLOWER_POWER_CLOSER_H
