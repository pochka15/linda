#include "LindaCoordinator.h"

#include <utility>

struct ReadRequest {
    std::string action;
    std::string listeningChannel;
    std::string pattern;
};

struct PublishRequest {
    std::string action;
    std::string tuple;
};

ReadRequest parseReadRequest(const std::string &data) {
    std::vector<std::string> lines;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return {lines[0], lines[1], lines[2]};
}

PublishRequest parsePublishRequest(const std::string &data) {
    std::vector<std::string> lines;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return {lines[0], lines[1]};
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
        communicationService.sendBlocking(request.listeningChannel, WRITER_COORDINATOR_CHANNEL);
    }
}

