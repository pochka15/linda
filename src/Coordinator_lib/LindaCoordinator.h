#ifndef LINDA_LINDACOORDINATOR_H
#define LINDA_LINDACOORDINATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>
#include <thread>
#include "LindaTupleUtils.h"
#include "CommunicationService.h"


struct Publisher {
    int tupleSize;
    std::string listeningChannel;
};

struct Reader {
    std::string pattern;
    std::string listeningChannel;
};

class LindaCoordinator {
public:
    explicit LindaCoordinator(const CommunicationService &communicationService);

    void handleRequests();

private:
    const CommunicationService &communicationService;
    const std::string COORDINATOR_CHANNEL = "Coordinator";
    std::vector<Publisher> publishers;
    std::vector<Reader> readers;

    void runReadScenario();

    void runPublishScenario();
};

#endif //LINDA_LINDACOORDINATOR_H
