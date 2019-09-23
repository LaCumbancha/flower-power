#ifndef FLOWER_POWER_SIGNALHANDLER_H
#define FLOWER_POWER_SIGNALHANDLER_H

#include <signal.h>
#include <stdio.h>
#include <cstdlib>
#include <memory.h>

#include "EventHandler.h"

class SignalHandler {

public:
    static SignalHandler *getInstance();
    static void destroy();
    int removeHandler(int signum);
    EventHandler *registerHandler(int signum, EventHandler *eh);

private:
    static SignalHandler *instance;
    static EventHandler *signal_handlers[NSIG];

    explicit SignalHandler() = default;
    static void dispatcher(int signum);

};

#endif // FLOWER_POWER_SIGNALHANDLER_H
