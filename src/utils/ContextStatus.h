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
    static void loadDataForResume();
    static bool retrieveSystemFinished();
    static void saveSystemFinishStatus(bool finished);

private:
    static Pipe* _incomingPipe;
    static Pipe* _retrievePipe;
    static bool _systemResumed;
    static bool _systemFinished;
    static std::string _stateFile;
    static std::map<std::string, std::string> _data;

    static bool isQuitIncoming(std::string &data);
    static bool isSaveIncoming(std::string &data);
    static bool isLoadIncoming(std::string &data);
    static bool isRetrieveIncoming(std::string &data);
    static bool isSaveSystemStatus(const std::string& data);
    static bool isSystemFinishedRequest(const std::string& data);

    static void loadData();
    static void sendSystemFinished();
    static void saveSystemStatus(const std::string& status);

    static void retrieveData(const std::string& id);
};


#endif //FLOWER_POWER_CONTEXTSTATUS_H
