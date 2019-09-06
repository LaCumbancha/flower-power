#include "System.h"

int System::run() {
    cout << "Running system with " << this->_config.getDistributionCenters() << " DC and " << this->_config.getSalePoints() << " SP." << endl;
    return EXIT_SUCCESS;
}

Config *System::getConfig() {
    return &_config;
}
