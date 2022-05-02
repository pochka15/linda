#include "LindaAgent.h"

#include <utility>

std::string buildPublishRequest(const std::string &publisherChannel, const Tuple &tuple) {
    return std::string("Publish\n") + formatTuple(tuple);
}

std::string buildReadRequest(const std::string &listeningChannel, const std::string &pattern) {
    return std::string("Read\n") + listeningChannel + "\n" + pattern;
}

LindaAgent::LindaAgent(std::string id, const CommunicationService &communicationService) :
        id(std::move(id)),
        communicationService(communicationService) {}

void LindaAgent::publishTupleBlocking(const Tuple &tuple) {
    cachedTuple = tuple;
    communicationService.sendBlocking(
            buildPublishRequest(READER_COORDINATOR_CHANNEL, tuple),
            WRITER_COORDINATOR_CHANNEL);
}

std::string LindaAgent::readBlocking(const std::string &pattern) {
    const std::string listeningChannel = READER_WRITER_CHANNEL;
    communicationService.sendBlocking(buildReadRequest(listeningChannel, pattern),
                                      READER_COORDINATOR_CHANNEL);
    return communicationService.receiveBlocking(listeningChannel);
}

void LindaAgent::handleRequestBlocking() {
//    It's assumed that coordinator just sends the listeningChannel to the writer
    const std::string &targetChannel = communicationService.receiveBlocking(WRITER_COORDINATOR_CHANNEL);
    communicationService.sendBlocking(formatTuple(cachedTuple), READER_WRITER_CHANNEL);
}
