#ifndef LINDA_LINDACOORDINATOR_H
#define LINDA_LINDACOORDINATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>
#include "LindaTupleUtils.h"
#include "CommunicationService.h"


class LindaCoordinator {
public:
    explicit LindaCoordinator(const CommunicationService &communicationService);

    void handleRequests();

    void getTupleFromWriter();

    void sendTuple();

private:
    const CommunicationService &communicationService;
    const std::string COORDINATOR_CHANNEL = "Coordinator";
    std::string rawReceivedTuple;
    std::string cachedReaderChannel;
    std::string cachedWriterChannel;
};

#endif //LINDA_LINDACOORDINATOR_H
