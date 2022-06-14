#include "LindaAgent.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <utility>

struct ReadRequest {
    std::string pattern;
    std::string channel;
    bool isVip;
};

struct MatchRequest {
    std::string pattern;
};

ReadRequest parseReadRequest(const std::string &data, bool isVip) {
    std::vector<std::string> lines = readLines(data);
    return {lines[1], lines[2], isVip};
}

MatchRequest parseMatchRequest(const std::string &data) {
    std::vector<std::string> lines = readLines(data);
    return {lines[1]};
}

std::string buildPublishRequest(const std::string &channel, unsigned long tupleSize) {
    return std::string("Publish\n")
           + std::to_string(tupleSize) + "\n"
           + channel;
}

std::string buildReadRequest(const std::string &channel, const std::string &pattern) {
    return std::string("Read\n")
           + pattern + "\n"
           + channel;
}

std::string buildVipReadRequest(const std::string &channel, const std::string &pattern) {
    return std::string("Read VIP\n")
           + pattern + "\n"
           + channel;
}

LindaAgent::LindaAgent(std::string privateChannel, const CommunicationService &communicationService) :
        privateChannel(std::move(privateChannel)),
        communicationService(communicationService) {}

void LindaAgent::publishTupleBlocking(const Tuple &tuple) {
    cachedTuple = tuple;
    communicationService.sendBlocking(
            buildPublishRequest(privateChannel, tuple.size()),
            COORDINATOR_CHANNEL);
}

std::string LindaAgent::readBlocking(const std::string &pattern) {
    communicationService.sendBlocking(
            buildReadRequest(privateChannel, pattern),
            COORDINATOR_CHANNEL);
    const std::string &data = communicationService.receiveBlocking(privateChannel);
    return data;
}

std::string LindaAgent::readVipBlocking(const std::string &pattern) {
    communicationService.sendBlocking(
            buildVipReadRequest(privateChannel, pattern),
            COORDINATOR_CHANNEL);
    const std::string &data = communicationService.receiveBlocking(privateChannel);
    return data;
}


// TODO It contains bug. It needs to be tested when parsing json for publisher with tuple ["10"]
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
        } else if (action["name"] == "readTupleVip") {
            stream << readVipBlocking(action["payload"]["pattern"].get<std::string>()) << '\n';
        } else if (action["name"] == "handleRequests") {
            handleRequestsBlocking();
        } else if (action["name"] == "terminate") {
            sleep(1);
            communicationService.sendBlocking("Terminate", COORDINATOR_CHANNEL);
        }
    }
    return stream.str();
}

/**
 * Handle all the requests from coordinator
 */
void LindaAgent::handleRequestsBlocking() {
    for (;;) {
        const std::string &data = communicationService.receiveBlocking(privateChannel);
        if (startsWith(data, "Read")) {
            bool isVip = startsWith(data, "Read VIP");
            const ReadRequest &request = parseReadRequest(data, isVip);
            communicationService.sendBlocking(formatTuple(cachedTuple), privateChannel);
            if (isVip) break;
        } else if (startsWith(data, "Match")) {
            const MatchRequest &request = parseMatchRequest(data);
            bool matches = checkIfMatches(cachedTuple, request.pattern);
            std::cout << (matches ? "OK" : "Bad") << std::endl;
            communicationService.sendBlocking(matches ? "OK" : "Bad", privateChannel);
        }
    }
}