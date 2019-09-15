#ifndef FLOWER_POWER_SYSTEM_H
#define FLOWER_POWER_SYSTEM_H

#include <zconf.h>
#include <sys/wait.h>
#include "config/Config.h"
#include "DistributionCenter.h"

class System {

public:
    explicit System() = default;

    pid_t run();
    Config* getConfig();
    void finish();

private:
    Config _config = Config();
    std::vector<pid_t> _distributionCentersPIDs;

};


#endif //FLOWER_POWER_SYSTEM_H
