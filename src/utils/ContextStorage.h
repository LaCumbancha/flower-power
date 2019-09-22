#ifndef FLOWER_POWER_CONTEXTSTORAGE_H
#define FLOWER_POWER_CONTEXTSTORAGE_H


#include <string>
#include "Pipe.h"
#include "Logger.h"

class ContextStorage {

public:
    explicit ContextStorage() = default;

    static void run();
    static void close();
    static void saveMode();
    static void saveContext(const std::string& data);
    static std::string retrieveContext(const std::string& id);

private:
    static Pipe* _incomingPipe;
    static Pipe* _retrievePipe;

    static bool isQuitIncoming(std::string &data);
    static bool isSaveIncoming(std::string &data);
    static bool isRetrieveIncoming(std::string &data);

};


#endif //FLOWER_POWER_CONTEXTSTORAGE_H
