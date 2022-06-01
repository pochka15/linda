#include <iostream>
#include "CommunicationService.h"
#include "LindaCoordinator.h"

void openCoordinatorChannel(const std::unique_ptr<CommunicationService> &service) {
    const std::string COORDINATOR_CHANNEL = "Coordinator";

    bool isChannelCreated = service->openChannel(COORDINATOR_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }
}


void closeCoordinatorChannel(const std::unique_ptr<CommunicationService> &service) {
    const std::string COORDINATOR_CHANNEL = "Coordinator";

    bool isChannelClosed = service->closeChannel(COORDINATOR_CHANNEL);
    if (!isChannelClosed) {
        std::cerr << "Couldn't close FIFO for the " << COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
    }
}


int main() {
    const auto &service = std::make_unique<CommunicationService>();
    openCoordinatorChannel(service);
    const auto &coordinator = std::make_unique<LindaCoordinator>(*service);
    coordinator->handleRequests();
    closeCoordinatorChannel(service);
    return 0;
}