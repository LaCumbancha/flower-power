#include "SellerJob.h"

SellerJob::SellerJob(Config* config) : Job(config) {};

int SellerJob::run() {
    cout << "Running seller job with " << _config->getSalePoints() << " SPs." << endl;
    return EXIT_SUCCESS;
}


