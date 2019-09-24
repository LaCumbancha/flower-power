#ifndef FLOWER_POWER_DISTRIBUTORJOB_H
#define FLOWER_POWER_DISTRIBUTORJOB_H


#include <utility>
#include <vector>
#include <map>
#include <signal.h>
#include "../Job.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../../utils/ContextStatus.h"
#include "../../config/data/SellerRequest.h"
#include "../../config/data/ClassifierBox.h"
#include "../../config/data/ClassifierBox.h"


class DistributorJob : public Job {

public:
    explicit DistributorJob(int center, Pipe *classifierPipe, Pipe *requestsPipe, std::map<std::string, Pipe*> distributionPipes);

    int run() override;
    int finish() override;
    int stopJob() override;

private:
    int _center;
    Pipe *_classifierPipe;
    Pipe *_requestsPipe;
    std::map<std::string, Pipe*> _distributionPipes;
    std::vector<ClassifierBox> _rosesStock;
    std::vector<ClassifierBox> _tulipsStock;

    bool resupplyPipeIsOpen = true;

    void handleRequest(const SellerRequest& request);
    void resupply(const SellerRequest& request);
    void takeClassifierBox();
    std::string contextState();

    void initializeStatus();

    void loadPreviousState(const string& previousState);
};


#endif //FLOWER_POWER_DISTRIBUTORJOB_H
