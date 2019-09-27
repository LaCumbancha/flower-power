#include "System.h"
#include "../utils/Closer.h"

int System::run() {
    // Set system as not finished
    ContextStatus::saveSystemFinishStatus(false);

    // Loading configuration data.
    this->_config.loadData();

    pid_t pid = getpid();
    Logger::info("Main system running in process with PID #" + std::to_string(pid) + ".");
    for (int idx = 1; idx <= this->_config.getDistributionCenters(); idx++) {

        // Creating new processes for each Distribution Center.
        if (pid != CHILD_PROCESS_PID) {
            pid = fork();

            // Creating a Distribution Center in each child process.
            if (pid == CHILD_PROCESS_PID) {
                pid = getpid();

                auto distributionCenter = new DistributionCenter(&(this->_config), idx);
                distributionCenter->run();
                distributionCenter->finish();

                Logger::info("Distribution Center #" + std::to_string(idx) + " running in process with PID #" + std::to_string(pid) + " destroyed.");
                ProcessKiller::removePID(pid);
                Closer::finishAuxJobs(pid);

                delete distributionCenter;
                exit(EXIT_SUCCESS);
            } else {
                Logger::info("Distribution Center #" + std::to_string(idx) + " running in process with PID #" + std::to_string(pid) + ".");
                this->_distributionCentersPIDs.push_back(pid);
                ProcessKiller::addPID(pid);
            }
        }

    }

    return EXIT_SUCCESS;
}

int System::finish() {
    int signals = 0;
    for (auto distributionCenter : this->_distributionCentersPIDs) {
        int processStatus;
        waitpid(distributionCenter, &processStatus, 0);

        if (processStatus != EXIT_SUCCESS) {
            signals += 1;
            Logger::error("Distribution Center in process " + std::to_string(distributionCenter) + " finished with error code " + std::to_string(processStatus));
        }
    }

    if (signals == 0) {
        Logger::info("Every Distribution Center finished successfully without errors.");
        Logger::info("System finished successfully.");
    }

    // Set system as finished
    ContextStatus::saveSystemFinishStatus(true);
}

Config *System::getConfig() {
    return &_config;
}
