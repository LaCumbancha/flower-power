#ifndef FLOWER_POWER_CONFIG_H
#define FLOWER_POWER_CONFIG_H


class Config {

public:
    int getSalePoints();
    int getDistributionCenters();
    void setSalePoints(int salePoints);
    void setDistributionCenters(int distributionCenters);

private:
    int _salePoints = 1;
    int _distributionCenters = 1;

};


#endif //FLOWER_POWER_CONFIG_H
