#include "ProducerJob.h"

ProducerJob::ProducerJob(Config* config) : Job(config) {};

int ProducerJob::run() {
    cout << "Running producer job with " << _config->getDistributionCenters() << " DCs. " << endl;
    return EXIT_SUCCESS;
}
