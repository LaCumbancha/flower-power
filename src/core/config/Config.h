#ifndef FLOWER_POWER_CONFIG_H
#define FLOWER_POWER_CONFIG_H

#include <list>
#include <vector>
#include <utility>
#include "data/FlowerBox.h"
#include "../../utils/csv/CSVReader.h"

#define CHILD_PROCESS_PID 0


class Config {

public:
    explicit Config() = default;
    void loadData();

    int getClients();
    int getSalePoints();
    int getDistributionCenters();
    std::vector<FlowerBox*> getProducers();

    void setClients(int clients);
    void setSalePoints(int salePoints);
    void setDistributionCenters(int distributionCenters);
    void setProducers(std::vector<FlowerBox*> producersDTO);

private:
    int _clients = 5;
    int _salePoints = 5;
    int _distributionCenters = 1;
    std::vector<FlowerBox*> _producers;

    std::string _producersPath = "./data/producers.csv";

};


#endif //FLOWER_POWER_CONFIG_H
