#include "Job.h"

int Job::run() {
    return EXIT_SUCCESS;
}

Job::Job(Config* config) {
    this->_config = config;
}
