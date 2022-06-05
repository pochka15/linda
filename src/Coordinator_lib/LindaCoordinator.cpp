#include "LindaCoordinator.h"

#include <utility>

struct ReadRequest {
    std::string pattern;
    std::string channel;
    bool isVip;

    [[nodiscard]] Reader toReader() const {
        return {pattern, channel, isVip};
    }
};

struct PublishRequest {
    int tupleSize;
    std::string channel;

    [[nodiscard]] Publisher toPublisher() const {
        return {tupleSize, channel};
    }
};

ReadRequest parseReadRequest(const std::string &data, bool isVip) {
    std::vector<std::string> lines = readLines(data);
    return {lines[1], lines[2], isVip};
}

PublishRequest parsePublishRequest(const std::string &data) {
    std::vector<std::string> lines = readLines(data);
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
            channelToReader[request.channel] = reader;
            collectionsMutex.unlock();

            std::thread(&LindaCoordinator::runReadScenario, this, reader)
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
 * Find publisher that is ready to send tuple that matches given pattern
 * @param pattern
 * @return found publisher or an empty structure
 */
Publisher LindaCoordinator::getPublisherByPattern(const std::string &pattern) {
    auto publisher_iter = std::find_if(
            publishers.begin(),
            publishers.end(),
            [this, &pattern](const Publisher &publisher) {
                communicationService.sendBlocking("Match\n" + pattern, publisher.channel);
                return communicationService.receiveBlocking(publisher.channel) == "OK";
            });
    if (publisher_iter == std::end(publishers)) return {};
    return *publisher_iter;
}

void LindaCoordinator::erasePublisherByChannel(const std::string &channel) {
    publishers.erase(std::remove_if(
                             publishers.begin(),
                             publishers.end(),
                             [&channel](Publisher &p) {
                                 return p.channel == channel;
                             }),
                     publishers.end());
}

std::vector<Reader> LindaCoordinator::getReadersCopy() {
    std::vector<Reader> readers;
    for (const auto &pair: channelToReader) {
        const Reader &reader = pair.second;
        readers.push_back({reader.pattern, reader.channel, reader.isVip});
    }
    return readers;
}


/**
 * Get a tuple from the publisher and send it back to the reader
 */
void LindaCoordinator::runReadScenario(const Reader &reader) {
//    Check if at list one publisher exists
    collectionsMutex.lock();
    bool noPublishers = publishers.empty();
    collectionsMutex.unlock();
    if (noPublishers) return;

//    Find matching publisher
    const std::lock_guard<std::mutex> lock(scenariosMutex);
    Publisher publisher = getPublisherByPattern(reader.pattern);
    if (publisher.channel.empty()) return;

//    Get tuple from publisher -> send it to the reader
    communicationService.sendBlocking(reader.isVip ? "Read VIP" : "Read", publisher.channel);
    const std::string &tuple = communicationService.receiveBlocking(publisher.channel);
    communicationService.sendBlocking(tuple, reader.channel);

//    Run cleanup logic
    collectionsMutex.lock();
    if (reader.isVip) erasePublisherByChannel(publisher.channel);
    channelToReader.erase(reader.channel);
    collectionsMutex.unlock();
}

void LindaCoordinator::runPublishScenario() {
    collectionsMutex.lock();
    std::vector<Reader> readers = getReadersCopy();
    collectionsMutex.unlock();
    for (const auto &reader: readers) runReadScenario(reader);
}
