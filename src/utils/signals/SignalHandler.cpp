#include "SignalHandler.h"

SignalHandler *SignalHandler::instance = nullptr;
EventHandler *SignalHandler::signal_handlers[NSIG];

SignalHandler *SignalHandler::getInstance() {

    if (instance == nullptr)
        instance = new SignalHandler();

    return instance;
}

void SignalHandler::destroy() {
    if (instance != nullptr) {
        delete (instance);
        instance = nullptr;
    }
}

EventHandler *SignalHandler::registerHandler(int signum, EventHandler *eh) {

    EventHandler *old_eh = SignalHandler::signal_handlers[signum];
    SignalHandler::signal_handlers[signum] = eh;

    struct sigaction sa{};
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SignalHandler::dispatcher;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, signum);
    sigaction(signum, &sa, 0);

    return old_eh;
}

void SignalHandler::dispatcher(int signum) {

    if (SignalHandler::signal_handlers[signum] != 0)
        SignalHandler::signal_handlers[signum]->handleSignal(signum);
}

int SignalHandler::removeHandler(int signum) {
    SignalHandler::signal_handlers[signum] = nullptr;
    return EXIT_SUCCESS;
}
