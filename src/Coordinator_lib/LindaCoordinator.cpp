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

    PublisherMeta toMeta() {
        return {tupleSize, listeningChannel};
    }
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
    for (;;) {
        const std::string &data = communicationService.receiveBlocking(COORDINATOR_CHANNEL);

//        Publish request
        if (startsWith(data, "Publish")) {
            auto request = parsePublishRequest(data);
            publishers.push_back(request.toMeta());
        }

//        Read request
        else if (startsWith(data, "Read")) {
            const ReadRequest &request = parseReadRequest(data);
            boost::thread t2(&LindaCoordinator::runScenario,
                             this,
                             request.pattern,
                             request.listeningChannel);
        }

//        Terminate
        else {
            std::cout << "Terminating" << std::endl;
            break;
        }
    }
}

/**
 * Hot-fix scenario. It's used in the reader request. It gets the tuple from the publisher and sends it back to the reader
 */
void LindaCoordinator::runScenario(const std::string &pattern, const std::string &readerChannel) {
    communicationService.sendBlocking(pattern, publishers[0].listeningChannel);
    const std::string &tuple = communicationService.receiveBlocking(publishers[0].listeningChannel);
    communicationService.sendBlocking(tuple, readerChannel);

//    Temporarily send terminate request to myself
    communicationService.sendBlocking("Terminate", COORDINATOR_CHANNEL);
}

bool startsWith(const std::string &text, const char *prefix) {
    return text.rfind(prefix, 0) == 0;
}
