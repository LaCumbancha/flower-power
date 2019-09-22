//
// Created by darius on 17/9/19.
//

#ifndef FLOWER_POWER_DISTRIBUTORJOB_H
#define FLOWER_POWER_DISTRIBUTORJOB_H


#include <utility>
#include <vector>
#include <map>
#include "../../../utils/Pipe.h"
#include "../Job.h"
#include "../../config/data/SellerRequest.h"
#include "../../config/data/ClassifierBox.h"

class DistributorJob : public Job {
private:
    int _centerId;
    Pipe *_classifierPipe;
    Pipe *_requestsPipe;
    std::map<std::string, Pipe*> _distributionPipes;
    std::vector<ClassifierBox> _rosesStock;
    std::vector<ClassifierBox> _tulipsStock;

    int _tulipsStockAmount = 0;
    void handleRequest(const SellerRequest& request);
    void resupply(const SellerRequest& request);
public:
    DistributorJob(int centerId, Pipe *classifierPipe, Pipe *requestsPipe, std::map<std::string, Pipe*> distributionPipes)
            : _classifierPipe(classifierPipe), _requestsPipe(requestsPipe),
              _distributionPipes(std::move(distributionPipes)),
              _centerId(centerId), Job() {};

    int run() override;
    int finish() override;
};


#endif //FLOWER_POWER_DISTRIBUTORJOB_H
