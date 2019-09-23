#include "StopHandler.h"

StopHandler::StopHandler(Job *job) : _job(job ) {}

int StopHandler::handleSignal(int signum) {
    this->_job->stopJob();
    SignalHandler::destroy();
    exit(EXIT_SUCCESS);
}
