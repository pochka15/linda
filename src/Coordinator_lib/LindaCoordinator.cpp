#include "LindaCoordinator.h"

#include <utility>

struct ReadRequest {
    std::string pattern;
    std::string listeningChannel;
};

struct PublishRequest {
    int tupleSize;
    std::string listeningChannel;
};

ReadRequest parseReadRequest(const std::string &data) {
    std::vector<std::string> lines;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return {lines[1], lines[2]};
}

PublishRequest parsePublishRequest(const std::string &data) {
    std::vector<std::string> lines;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    int tupleSize = 0;
    try {
        tupleSize = std::stoi(lines[1]);
    } catch (std::exception &ignored) {}
    
    return {tupleSize, lines[2]};
}

LindaCoordinator::LindaCoordinator(const CommunicationService &communicationService) :
        communicationService(communicationService) {}

void LindaCoordinator::handleRequestBlocking() {
    // Now we temporarily assume that requests firstly come from the writer then from reader
    if (shouldHandleRequestsFromWriter) {
        const std::string &data = communicationService.receiveBlocking(WRITER_COORDINATOR_CHANNEL);
        auto request = parsePublishRequest(data);
        shouldHandleRequestsFromWriter = false;
    } else { // request from reader
        const std::string &data = communicationService.receiveBlocking(READER_COORDINATOR_CHANNEL);
        const ReadRequest &request = parseReadRequest(data);
        communicationService.sendBlocking(request.pattern, WRITER_COORDINATOR_CHANNEL);
    }
}

/**
 * Hot-fix method. Later it will be refactored
 */
void LindaCoordinator::getTupleFromWriter() {
    rawReceivedTuple = communicationService.receiveBlocking(WRITER_COORDINATOR_CHANNEL);
}

/**
 * Hot-fix method. Later it will be refactored
 */
 void LindaCoordinator::sendTuple() {
    communicationService.sendBlocking(rawReceivedTuple, READER_COORDINATOR_CHANNEL);
}

