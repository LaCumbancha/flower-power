#ifndef FLOWER_POWER_CONFIG_H
#define FLOWER_POWER_CONFIG_H

#include <list>
#include <vector>
#include "data/ProducerFlowers.h"

#define CHILD_PROCESS_PID 0


class Config {

public:
    explicit Config() = default;
    void loadData();
    int getSalePoints();
    int getDistributionCenters();
    void setSalePoints(int salePoints);
    void setDistributionCenters(int distributionCenters);
    std::vector<ProducerFlowers> getProducers();
    void setProducers(std::vector<ProducerFlowers> producersDTO);

private:
    int _salePoints = 1;
    int _distributionCenters = 1;
    std::string _producersPath = "../data/producers.csv";
    std::vector<ProducerFlowers> _producers;

};


#endif //FLOWER_POWER_CONFIG_H
