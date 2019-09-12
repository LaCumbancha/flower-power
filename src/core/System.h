#ifndef FLOWER_POWER_SYSTEM_H
#define FLOWER_POWER_SYSTEM_H


#include <zconf.h>
#include "config/Config.h"
#include "DistributionCenter.h"


class System {

public:
    explicit System() = default;

    pid_t run();
    Config* getConfig();

private:
    Config _config = Config();

};


#endif //FLOWER_POWER_SYSTEM_H
