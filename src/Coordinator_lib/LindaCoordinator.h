#ifndef LINDA_LINDACOORDINATOR_H
#define LINDA_LINDACOORDINATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>
#include <boost/thread.hpp>
#include "LindaTupleUtils.h"
#include "CommunicationService.h"


struct PublisherMeta {
    int tupleSize;
    std::string listeningChannel;
};

class LindaCoordinator {
public:
    explicit LindaCoordinator(const CommunicationService &communicationService);

    void handleRequests();

private:
    const CommunicationService &communicationService;
    const std::string COORDINATOR_CHANNEL = "Coordinator";
    std::vector<PublisherMeta> publishers;

    void runScenario(const std::string &pattern, const std::string &readerChannel);
};

#endif //LINDA_LINDACOORDINATOR_H
