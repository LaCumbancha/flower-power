#ifndef FLOWER_POWER_CONFIG_H
#define FLOWER_POWER_CONFIG_H

#include <list>
#include <vector>
#include "data/ProducerFlowers.h"
#include "data/Seller.h"

#define CHILD_PROCESS_PID 0


class Config {

public:
    explicit Config() = default;
    void loadData();
    int getDistributionCenters();
    void setDistributionCenters(int distributionCenters);
    std::vector<ProducerFlowers> getProducers();
    void setProducers(std::vector<ProducerFlowers> producersDTO);
    std::vector<Seller> getSalePoints();
    void setSellers(std::vector<Seller> sellers);

private:
    int _distributionCenters = 1;
    std::string _producersPath = "../data/producers.csv";
    std::string _sellersPath = "../data/sellers.csv";
    std::vector<ProducerFlowers> _producers;
    std::vector<Seller> _sellers;

};


#endif //FLOWER_POWER_CONFIG_H
