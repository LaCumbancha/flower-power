#include "System.h"

pid_t System::run() {
    // Loading configuration data.
    this->_config.loadData();

    pid_t pid = getpid();
    for (int idx = 0; idx < this->_config.getDistributionCenters(); idx++) {

        // Creating new processes for each Distribution Center.
        if (pid != CHILD_PROCESS_PID) {
            pid = fork();

            // Creating a Distribution Center in each child process.
            if (pid == CHILD_PROCESS_PID) {
                auto distributionCenter = DistributionCenter(&(this->_config));
                distributionCenter.run();
            }
        }

    }

    return pid;
}

Config *System::getConfig() {
    return &_config;
}
