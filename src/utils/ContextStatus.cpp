#include "ContextStatus.h"
#include "csv/CSVWriter.h"
#include "csv/CSVReader.h"

bool ContextStatus::_systemResumed = false;
bool ContextStatus::_systemFinished = false;
Pipe* ContextStatus::_incomingPipe = new Pipe();
Pipe* ContextStatus::_retrievePipe = new Pipe();
std::string ContextStatus::_stateFile = "./tmp/state.csv";
std::map<std::string, std::string> ContextStatus::_data = std::map<std::string, std::string>();

void ContextStatus::run() {
    std::string data;
    int status;

    // Creating CSV Writer.
    auto writer = new CSVWriter(ContextStatus::_stateFile);
    writer->clearFile();

    // Reading incoming stats.
    ContextStatus::_incomingPipe->setReadMode();
    ContextStatus::_retrievePipe->setWriteMode();
    while (ContextStatus::_incomingPipe->read(data, &status)) {
        if (status == EXIT_SUCCESS) {
            if (isSaveIncoming(data)) {
                writer->saveData(data.substr(3, data.size()));
            } else if (isLoadIncoming(data)) {
                loadData();
                writer->clearFile();
            } else if (isRetrieveIncoming(data)) {
                retrieveData(data.substr(3, data.size()));
            } else if (isSaveSystemStatus(data)) {
                saveSystemStatus(data);
            } else if (isSystemFinishedRequest(data)) {
                sendSystemFinished();
            } else if (isQuitIncoming(data)) {
                break;
            }
        }
    }

    Logger::info("Closing Context Storage.");
    ContextStatus::_incomingPipe->~Pipe();
    ContextStatus::_retrievePipe->~Pipe();
    exit(EXIT_SUCCESS);
}

void ContextStatus::close() {
    ContextStatus::_incomingPipe->write("QUIT");
    ContextStatus::_incomingPipe->~Pipe();
    ContextStatus::_retrievePipe->~Pipe();
}

void ContextStatus::saveMode() {
    ContextStatus::_incomingPipe->setWriteMode();
    ContextStatus::_retrievePipe->setReadMode();
}

void ContextStatus::saveContext(const std::string& data) {
    ContextStatus::_incomingPipe->write("SC|" + data);
}

void ContextStatus::loadDataForResume() {
    ContextStatus::_incomingPipe->write("LD|");
}

std::string ContextStatus::retrieveContext(const std::string& id) {
    ContextStatus::_incomingPipe->write("RC|" + id);

    std::string data;
    int status;

    ContextStatus::_retrievePipe->read(data, &status);
    if (status == EXIT_SUCCESS) return data;

    Logger::error("Context Storage failed to retrieve data for id " + id + ".");
    return "";
}

void ContextStatus::saveSystemFinishStatus(bool finished) {
    if (finished) ContextStatus::_incomingPipe->write("SS|FINISHED");
    else ContextStatus::_incomingPipe->write("SS|RUNNING");
}

bool ContextStatus::retrieveSystemFinished() {
    ContextStatus::_incomingPipe->write("SF|");

    std::string data;
    int status;

    ContextStatus::_retrievePipe->read(data, &status);
    if (status == EXIT_SUCCESS) {
        if (data == "FINISHED") return true;
        if (data == "RUNNING") return false;
    }

    Logger::error("Context Storage failed to retrieve system status.");
    return true;
}

bool ContextStatus::isQuitIncoming(std::string &data) {
    return data == "QUIT";
}

bool ContextStatus::isLoadIncoming(std::string &data) {
    return data.substr(0, 3) == "LD|";
}

bool ContextStatus::isSaveIncoming(std::string &data) {
    return data.substr(0, 3) == "SC|";
}

bool ContextStatus::isRetrieveIncoming(std::string &data) {
    return data.substr(0, 3) == "RC|";
}

bool ContextStatus::isSaveSystemStatus(const std::string& data) {
    return data.substr(0, 3) == "SS|";
}

bool ContextStatus::isSystemFinishedRequest(const std::string& data) {
    return data.substr(0, 3) == "SF|";
}

void ContextStatus::saveSystemStatus(const std::string& status) {
    if (status.substr(3, 8) == "FINISHED") ContextStatus::_systemFinished = true;
    else if (status.substr(3, 7) == "RUNNING") ContextStatus::_systemFinished = false;
}

void ContextStatus::sendSystemFinished() {
    if (ContextStatus::_systemFinished) {
        ContextStatus::_retrievePipe->write("FINISHED");
    } else {
        ContextStatus::_retrievePipe->write("RUNNING");
    }
}

void ContextStatus::loadData() {

    // Setting resume flag on.
    ContextStatus::_systemResumed = true;

    std::string buffer;

    // Loading producers from CSV.
    auto reader = new CSVReader(ContextStatus::_stateFile);
    std::vector<std::string> stateData = reader->getDataByLines();

    for (const auto& line : stateData) {
        buffer = "";

        for (auto character : line) {

            if (character == ',') {
                Logger::warn("Job "+ buffer + " stored: [" + line.substr(buffer.size() + 1) + "].");
                ContextStatus::_data.insert(std::pair<std::string, std::string>(buffer, line.substr(buffer.size() + 1)));
                break;
            } else {
                buffer += character;
            }

        }
    }

    Logger::info("Data retrieved to resume system: " + std::to_string(ContextStatus::_data.size()) + " records.");

}

void ContextStatus::retrieveData(const std::string& id) {

    if (ContextStatus::_systemResumed) {
        // Retrieving data for previous state.
        if (ContextStatus::_data.find(id) == ContextStatus::_data.end()) {
            Logger::error("Failed to retrieve previous state data for Job " + id + ".");
            ContextStatus::_retrievePipe->write("");
        } else {
            Logger::warn("Job "+ id + " retrieved: [" + ContextStatus::_data.find(id)->second + "].");
            ContextStatus::_retrievePipe->write(ContextStatus::_data.find(id)->second);
        }
    } else {
        ContextStatus::_retrievePipe->write("");
    }

}
