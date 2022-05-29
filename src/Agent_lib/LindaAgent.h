#ifndef LINDA_LINDAAGENT_H
#define LINDA_LINDAAGENT_H

#include "LindaTupleUtils.h"
#include "CommunicationService.h"
#include "nlohmann/json.hpp"

class LindaAgent {
public:
    const std::string privateChannel;

    LindaAgent(std::string privateChannel, const CommunicationService &communicationService);

    void publishTupleBlocking(const Tuple &tuple);

    void handleRequestBlocking();

    std::string readBlocking(const std::string &pattern);

    std::string executeScenario(const nlohmann::basic_json<> &scenario);

private:
    const CommunicationService &communicationService;
    const std::string COORDINATOR_CHANNEL = "Coordinator";
    Tuple cachedTuple = Tuple();
};


#endif //LINDA_LINDAAGENT_H
