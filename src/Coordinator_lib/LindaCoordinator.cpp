#include "LindaCoordinator.h"

#include <utility>

struct ReadRequest {
    std::string pattern;
    std::string listeningChannel;
    bool isVip;

    [[nodiscard]] Reader toReader() const {
        return {pattern, listeningChannel, isVip};
    }
};

std::string buildReadRequest(const std::string &listeningChannel, const std::string &pattern) {
    return {"Read\n"};
}

std::string buildVipReadRequest(const std::string &listeningChannel, const std::string &pattern) {
    return {"Read VIP\n"};
}

struct PublishRequest {
    int tupleSize;
    std::string listeningChannel;

    [[nodiscard]] Publisher toPublisher() const {
        return {tupleSize, listeningChannel};
    }
};

ReadRequest parseReadRequest(const std::string &data, bool isVip) {
    std::vector<std::string> lines;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return {lines[1], lines[2], isVip};
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
            const PublishRequest &request = parsePublishRequest(data);
            publishers.push_back(request.toPublisher());
            std::thread(&LindaCoordinator::runPublishScenario, this)
                    .detach();
        }

//        Read and 'Read VIP' requests
        else if (startsWith(data, "Read")) {
            const ReadRequest &request = parseReadRequest(
                    data,
                    startsWith(data, "Read VIP"));
            channelToReader[request.listeningChannel] = request.toReader();
            std::thread(&LindaCoordinator::runReadScenario, this)
                    .detach();
        }

//        Terminate
        else if (startsWith(data, "Terminate")) {
            std::cout << "Terminating" << std::endl;
            break;
        }
    }
}


/**
 * Get a tuple from the publisher and send it back to the reader
 */
void LindaCoordinator::runReadScenario() {
    if (publishers.empty()) return;

    Reader &reader = channelToReader.begin()->second;

    communicationService.sendBlocking(
            reader.isVip ? "Read VIP" : "Read",
            publishers[0].listeningChannel);

    const std::string &tuple = communicationService.receiveBlocking(publishers[0].listeningChannel);
    communicationService.sendBlocking(tuple, reader.listeningChannel);

    if (reader.isVip) publishers.clear();
    channelToReader.clear();
}

void LindaCoordinator::runPublishScenario() {
    if (!channelToReader.empty()) runReadScenario();
}
