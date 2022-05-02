#ifndef LINDA_LINDAAGENT_H
#define LINDA_LINDAAGENT_H


#include "Common_lib/LindaTupleUtils.h"
#include "Common_lib/CommunicationService.h"

class LindaAgent {
public:
    const std::string id;

    LindaAgent(std::string id, const CommunicationService &communicationService);

    void publishTupleBlocking(const Tuple &tuple);

    void handleRequestBlocking();

    std::string readBlocking(const std::string &pattern);

private:
    const CommunicationService &communicationService;
    const std::string WRITER_COORDINATOR_CHANNEL = "Writer-Coordinator";
    const std::string READER_COORDINATOR_CHANNEL = "Reader-Coordinator";
    const std::string READER_WRITER_CHANNEL = "Reader-Writer";
    Tuple cachedTuple = Tuple();
};


#endif //LINDA_LINDAAGENT_H
