#ifndef FLOWER_POWER_STOPHANDLER_H
#define FLOWER_POWER_STOPHANDLER_H


#include "../../core/job/Job.h"
#include "EventHandler.h"
#include "SignalHandler.h"

class StopHandler : public EventHandler {

public:
    explicit StopHandler(Job* job);
    int handleSignal(int signum) override;

private:
    Job* _job;

};


#endif //FLOWER_POWER_STOPHANDLER_H
