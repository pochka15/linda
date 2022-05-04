#include "LindaAgent.h"

#include <utility>

std::string buildPublishRequest(const std::string &listeningChannel, const Tuple &tuple) {
    return std::string("Publish\n")
           + std::to_string(tuple.size()) + "\n"
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
            buildPublishRequest(WRITER_COORDINATOR_CHANNEL, tuple),
            WRITER_COORDINATOR_CHANNEL);
}

std::string LindaAgent::readBlocking(const std::string &pattern) {
    const std::string &channel = READER_COORDINATOR_CHANNEL;
    communicationService.sendBlocking(buildReadRequest(channel, pattern), channel);
    return communicationService.receiveBlocking(channel);
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
