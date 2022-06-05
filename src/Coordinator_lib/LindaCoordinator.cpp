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
            const Publisher &publisher = request.toPublisher();

            collectionsMutex.lock();
            publishers.push_back(publisher);
            collectionsMutex.unlock();

            std::thread(&LindaCoordinator::runPublishScenario, this)
                    .detach();
        }

//        Read and 'Read VIP' requests
        else if (startsWith(data, "Read")) {
            const ReadRequest &request = parseReadRequest(
                    data,
                    startsWith(data, "Read VIP"));
            const Reader &reader = request.toReader();

            collectionsMutex.lock();
            channelToReader[request.listeningChannel] = reader;
            collectionsMutex.unlock();

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
//    Check if publisher exists
    collectionsMutex.lock();
    bool noPublishers = publishers.empty();
    collectionsMutex.unlock();
    if (noPublishers) return;

//    Get tuple from publisher -> send to the reader
    const std::lock_guard<std::mutex> lock(scenariosMutex);
    Reader &reader = channelToReader.begin()->second;
    communicationService.sendBlocking(
            reader.isVip ? "Read VIP" : "Read",
            publishers[0].listeningChannel);
    const std::string &tuple = communicationService.receiveBlocking(publishers[0].listeningChannel);
    communicationService.sendBlocking(tuple, reader.listeningChannel);

//    Run cleanup logic
    collectionsMutex.lock();
    if (reader.isVip) publishers.clear();
    channelToReader.clear();
    collectionsMutex.unlock();
}

void LindaCoordinator::runPublishScenario() {
    collectionsMutex.lock();
    bool readerExists = !channelToReader.empty();
    collectionsMutex.unlock();
    if (readerExists) runReadScenario();
}
