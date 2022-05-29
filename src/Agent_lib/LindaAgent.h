#ifndef LINDA_LINDAAGENT_H
#define LINDA_LINDAAGENT_H

#include "LindaTupleUtils.h"
#include "CommunicationService.h"
#include "nlohmann/json.hpp"

class LindaAgent {
public:
    const std::string id;

    LindaAgent(std::string id, const CommunicationService &communicationService);

    void publishTupleBlocking(const Tuple &tuple);

    void handleRequestBlocking();

    std::string readBlocking(const std::string &pattern);

    std::string executeScenario(const nlohmann::basic_json<> &scenario);

private:
    const CommunicationService &communicationService;
    const std::string WRITER_COORDINATOR_CHANNEL = "Writer-Coordinator";
    const std::string READER_COORDINATOR_CHANNEL = "Reader-Coordinator";
    const std::string READER_WRITER_CHANNEL = "Reader-Writer";
    Tuple cachedTuple = Tuple();

    bool openChannel() const; // NOLINT(modernize-use-nodiscard)
    bool closeChannel() const; // NOLINT(modernize-use-nodiscard)
};


#endif //LINDA_LINDAAGENT_H
