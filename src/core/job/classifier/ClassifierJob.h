#ifndef FLOWER_POWER_CLASSIFIERJOB_H
#define FLOWER_POWER_CLASSIFIERJOB_H


#include <vector>
#include <iostream>
#include <signal.h>
#include "../Job.h"
#include "../../config/data/Flower.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../../utils/ContextStatus.h"

class ClassifierJob : public Job {

public:
    explicit ClassifierJob(int center, Pipe* producersPipe, Pipe* distributorPipe);
    int run() override;
    ~ClassifierJob();
private:

    int _center;
    Pipe* _producersPipe;
    Pipe* _distributorPipe;
    std::vector<Flower> _roses;
    std::vector<Flower> _tulips;
    std::string contextState();
    int finish() override;
    __sighandler_t handler();

};


#endif //FLOWER_POWER_CLASSIFIERJOB_H
