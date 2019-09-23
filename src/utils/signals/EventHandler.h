#ifndef FLOWER_POWER_EVENTHANDLER_H
#define FLOWER_POWER_EVENTHANDLER_H

class EventHandler {

public:
    virtual int handleSignal(int signum) = 0;

    virtual ~EventHandler() = default;
};

#endif /* FLOWER_POWER_EVENTHANDLER_H */
