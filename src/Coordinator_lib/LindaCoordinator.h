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

    void handleRequestBlocking();

    void getTupleFromWriter();

    void sendTuple();

private:
    const CommunicationService &communicationService;
    const std::string WRITER_COORDINATOR_CHANNEL = "Writer-Coordinator";
    const std::string READER_COORDINATOR_CHANNEL = "Reader-Coordinator";
    bool shouldHandleRequestsFromWriter = true;
    std::string rawReceivedTuple;
};

#endif //LINDA_LINDACOORDINATOR_H
