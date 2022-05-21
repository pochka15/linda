#include "LindaAgent.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
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
    const std::string &channel = READER_COORDINATOR_CHANNEL;
    communicationService.sendBlocking(buildReadRequest(channel, pattern), channel);
    return communicationService.receiveBlocking(channel);
}

std::string LindaAgent::executeScenario(const nlohmann::basic_json<> &scenario) {
    const std::string &pattern = scenario["actions"][0]["payload"]["pattern"].dump();
//    TODO for each parsed action: exec action one by one
    return readBlocking(pattern);
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
