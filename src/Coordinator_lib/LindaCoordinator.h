#ifndef LINDA_LINDACOORDINATOR_H
#define LINDA_LINDACOORDINATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>
#include <thread>
#include <mutex>
#include <map>
#include "LindaTupleUtils.h"
#include "CommunicationService.h"
#include "StringUtils.h"


struct Publisher {
    int tupleSize;
    std::string channel;
};

struct Reader {
    std::string pattern;
    std::string channel;
    bool isVip;
};

class LindaCoordinator {
public:
    explicit LindaCoordinator(const CommunicationService &communicationService);

    void handleRequests();

private:
    const CommunicationService &communicationService;
    const std::string COORDINATOR_CHANNEL = "Coordinator";
    std::vector<Publisher> publishers;
    std::map<std::string, Reader> channelToReader;
    std::mutex collectionsMutex;
    std::mutex scenariosMutex;

    void runReadScenario(const Reader &reader);

    void runPublishScenario();

    Publisher getPublisherByPattern(const std::string &pattern);

    void erasePublisherByChannel(const std::string &channel);

    std::vector<Reader> getReadersCopy();
};

#endif //LINDA_LINDACOORDINATOR_H
