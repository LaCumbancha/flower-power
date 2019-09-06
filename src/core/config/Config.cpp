#include "Config.h"

int Config::getSalePoints() {
    return _salePoints;
}

int Config::getDistributionCenters() {
    return _distributionCenters;
}

void Config::setSalePoints(int salePoints) {
    this->_salePoints = salePoints;
}

void Config::setDistributionCenters(int distributionCenters) {
    this->_distributionCenters = distributionCenters;
}
