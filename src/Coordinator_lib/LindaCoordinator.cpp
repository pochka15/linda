#include "LindaCoordinator.h"

#include <utility>

bool startsWith(const std::string &text, const char *prefix);

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

void LindaCoordinator::handleRequests() {
//    We temporarily handle only two requests
    for (int i = 0; i < 2; ++i) {
        const std::string &data = communicationService.receiveBlocking(COORDINATOR_CHANNEL);

//        Publish request
        if (startsWith(data, "Publish")) {
            auto request = parsePublishRequest(data);
            cachedWriterChannel = request.listeningChannel;
        }

//        Read request
        else {
            const ReadRequest &request = parseReadRequest(data);
            cachedReaderChannel = request.listeningChannel;
            communicationService.sendBlocking(request.pattern, cachedWriterChannel);
        }
    }
}

/**
 * Hot-fix method. Later it will be refactored
 */
void LindaCoordinator::getTupleFromWriter() {
    rawReceivedTuple = communicationService.receiveBlocking(cachedWriterChannel);
}

/**
 * Hot-fix method. Later it will be refactored
 */
void LindaCoordinator::sendTuple() {
    communicationService.sendBlocking(rawReceivedTuple, cachedReaderChannel);
}

bool startsWith(const std::string &text, const char *prefix) {
    return text.rfind(prefix, 0) == 0;
}
