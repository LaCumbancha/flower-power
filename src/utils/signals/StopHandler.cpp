#include "StopHandler.h"
#include "../Closer.h"

StopHandler::StopHandler(Job *job) : _job(job ) {}

int StopHandler::handleSignal(int signum) {
    this->_job->stopJob();
    delete this->_job;

    ProcessKiller::removePID(getpid());
    Closer::finishAuxJobs(getpid());

    SignalHandler::destroy();
    exit(EXIT_SUCCESS);
}
