#ifndef FLOWER_POWER_CONTEXTSTATUS_H
#define FLOWER_POWER_CONTEXTSTATUS_H


#include <string>
#include "Pipe.h"
#include "Logger.h"


class ContextStatus {

public:
    explicit ContextStatus() = default;

    static void run();
    static void close();
    static void saveMode();
    static void saveContext(const std::string& data);
    static std::string retrieveContext(const std::string& id);
    static bool retrieveSystemFinished();
    static void saveSystemFinishStatus(bool finished);

private:
    static Pipe* _incomingPipe;
    static Pipe* _retrievePipe;
    static bool _systemFinished;

    static bool isQuitIncoming(std::string &data);
    static bool isSaveIncoming(std::string &data);
    static bool isRetrieveIncoming(std::string &data);
    static bool isSaveSystemStatus(const std::string& data);
    static bool isSystemFinishedRequest(const std::string& data);

    static void sendSystemFinished();
    static void saveSystemStatus(const std::string& status);

};


#endif //FLOWER_POWER_CONTEXTSTATUS_H
