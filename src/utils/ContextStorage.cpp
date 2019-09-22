#include "ContextStorage.h"
#include "csv/CSVWriter.h"

Pipe* ContextStorage::_incomingPipe = new Pipe();
Pipe* ContextStorage::_retrievePipe = new Pipe();

void ContextStorage::run() {
    std::string data;
    int status;

    // Creating CSV Writer.
    auto writer = new CSVWriter("/tmp/state.csv");

    // Reading incoming stats.
    ContextStorage::_incomingPipe->setReadMode();
    ContextStorage::_retrievePipe->setWriteMode();
    while (ContextStorage::_incomingPipe->read(data, &status)) {
        if (status == EXIT_SUCCESS) {
            if (isSaveIncoming(data)) {
                writer->saveData(data);
            } else if (isRetrieveIncoming(data)) {
                // TODO: Not implemented.
            } else if (isQuitIncoming(data)) {
                break;
            }
        }
    }

    Logger::info("Closing Context Storage.");
    ContextStorage::_incomingPipe->~Pipe();
    ContextStorage::_retrievePipe->~Pipe();
    exit(EXIT_SUCCESS);
}

void ContextStorage::close() {
    ContextStorage::_incomingPipe->write("QUIT");
    ContextStorage::_incomingPipe->~Pipe();
    ContextStorage::_retrievePipe->~Pipe();
}

void ContextStorage::saveMode() {
    ContextStorage::_incomingPipe->setWriteMode();
    ContextStorage::_retrievePipe->setReadMode();
}

void ContextStorage::saveContext(const std::string& data) {
    ContextStorage::_incomingPipe->write("S|" + data);
}

std::string ContextStorage::retrieveContext(const std::string& id) {
    ContextStorage::_incomingPipe->write("R|" + id);

    std::string data;
    int status;

    ContextStorage::_retrievePipe->read(data, &status);
    if (status == EXIT_SUCCESS) return data;

    Logger::error("Context Storage failed to retrieve data for id " + id + ".");
    return "";
}

bool ContextStorage::isQuitIncoming(std::string &data) {
    return data == "QUIT";
}

bool ContextStorage::isSaveIncoming(std::string &data) {
    return data.substr(0, 2) == "S|";
}

bool ContextStorage::isRetrieveIncoming(std::string &data) {
    return data.substr(0, 2) == "R|";
}
