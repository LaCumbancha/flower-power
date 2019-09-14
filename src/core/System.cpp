#include "System.h"
#include "../utils/Logger.h"

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
            } else {
                this->_distributionCenters.push_back(pid);
            }
        }

    }

    finish();
    return EXIT_SUCCESS;
}

void System::finish() {
    for (auto distributionCenter : this->_distributionCenters) {
        int processStatus;
        waitpid(distributionCenter, &processStatus, 0);

        if (processStatus != EXIT_SUCCESS) {
            Logger::error("Process " + std::to_string(distributionCenter) + " finished with error code " + std::to_string(processStatus));
        }
    }
}

Config *System::getConfig() {
    return &_config;
}
