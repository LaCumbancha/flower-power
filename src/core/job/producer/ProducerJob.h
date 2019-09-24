#ifndef FLOWER_POWER_PRODUCERJOB_H
#define FLOWER_POWER_PRODUCERJOB_H


#include <iostream>
#include <csignal>
#include "../Job.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../../utils/ContextStatus.h"
#include "../../../utils/csv/CSVWriter.h"


class ProducerJob : public Job {

public:
    explicit ProducerJob(int centerId, const FlowerBox* producerData, Pipe* producerPipe);
    int run() override;
    int stopJob() override;
    ~ProducerJob();
private:

    int _centerId;
    int _producerId;
    int _rosesStock{};
    int _tulipsStock{};
    std::string _producerName;
    Pipe* _producerPipe;

    FlowerBox generateFlowerBox();
    int finish() override;
    std::string contextState();

    void initializeStatus(const FlowerBox *producerData);

    void loadPreviousState(const string& previousState);
};


#endif //FLOWER_POWER_PRODUCERJOB_H
