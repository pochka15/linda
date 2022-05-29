#include "LindaAgent.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <utility>

std::string buildPublishRequest(const std::string &listeningChannel, unsigned long tupleSize) {
    return std::string("Publish\n")
           + std::to_string(tupleSize) + "\n"
           + listeningChannel;
}

std::string buildReadRequest(const std::string &listeningChannel, const std::string &pattern) {
    return std::string("Read\n")
           + pattern + "\n"
           + listeningChannel;

}

LindaAgent::LindaAgent(std::string id, const CommunicationService &communicationService) :
        id(std::move(id)),
        communicationService(communicationService) {}

void LindaAgent::publishTupleBlocking(const Tuple &tuple) {
    cachedTuple = tuple;
    communicationService.sendBlocking(
            buildPublishRequest(WRITER_COORDINATOR_CHANNEL, tuple.size()),
            WRITER_COORDINATOR_CHANNEL);
}

std::string LindaAgent::readBlocking(const std::string &pattern) {
    if (!openChannel()) return "";

    const std::string &channel = READER_COORDINATOR_CHANNEL;
    communicationService.sendBlocking(buildReadRequest(channel, pattern), channel);
    const std::string &data = communicationService.receiveBlocking(channel);

    closeChannel();
    return data;
}

Tuple parseTuple(const nlohmann::basic_json<> &json) {
    Tuple t;
    for (const auto &it: json) {
        if (it.is_number_float()) t.push_back(it.get<float>());
        if (it.is_number_integer()) t.push_back(it.get<int>());
        if (it.is_string()) t.push_back(it.get<std::string>());
    }
    return t;
}

std::string LindaAgent::executeScenario(const nlohmann::basic_json<> &scenario) {
    std::stringstream stream;
    for (const auto &action: scenario["actions"]) {
        if (action["name"] == "publishTuple") {
            publishTupleBlocking(parseTuple(action["payload"]["tuple"]));
        } else if (action["name"] == "readTuple") {
            stream << readBlocking(action["payload"]["pattern"].get<std::string>()) << '\n';
        }
    }
    return stream.str();
}

/**
 * Handle all the requests from coordinator
 */
void LindaAgent::handleRequestBlocking() {
//    For now, this function is only used by the writer. It just accepts any message from the coordinator
//    and after that immediately sends the tuple
    const std::string &pattern = communicationService.receiveBlocking(WRITER_COORDINATOR_CHANNEL);
    communicationService.sendBlocking(formatTuple(cachedTuple), WRITER_COORDINATOR_CHANNEL);
}

bool LindaAgent::openChannel() const {
    bool isChannelCreated = communicationService.openChannel(id);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << id << " : " << strerror(errno) << std::endl;
    }
    return isChannelCreated;
}

bool LindaAgent::closeChannel() const {
    bool isChannelClosed = communicationService.closeChannel(id);
    if (!isChannelClosed) {
        std::cerr << "Couldn't close FIFO for the " << id
                  << " : " << strerror(errno) << std::endl;
    }
    return isChannelClosed;
}