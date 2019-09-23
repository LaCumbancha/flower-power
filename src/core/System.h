#ifndef FLOWER_POWER_SYSTEM_H
#define FLOWER_POWER_SYSTEM_H

#include <zconf.h>
#include <sys/wait.h>
#include "config/Config.h"
#include "DistributionCenter.h"
#include "../utils/ProcessKiller.h"

class System {

public:
    explicit System() = default;

    int run();
    int finish();
    Config* getConfig();

private:
    Config _config = Config();
    std::vector<pid_t> _distributionCentersPIDs;

};


#endif //FLOWER_POWER_SYSTEM_H
