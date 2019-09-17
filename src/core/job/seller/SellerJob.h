#ifndef FLOWER_POWER_SELLERJOB_H
#define FLOWER_POWER_SELLERJOB_H


#include <iostream>
#include "../Job.h"
#include "../../config/Config.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"

class SellerJob : public Job {

public:
    explicit SellerJob(const int center, const Seller& sellerData, Pipe* requestPipe, Pipe* distributionPipe);
    int run() override;

private:
    int _center;
    int _sellerId;
    int _rosesStock;
    int _tulipsStock;
    std::string _sellerName;
    Pipe* _distributionPipe;
    Pipe* _requestPipe;

};


#endif //FLOWER_POWER_SELLERJOB_H
