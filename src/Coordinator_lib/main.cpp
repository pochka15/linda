#include <iostream>
#include "CommunicationService.h"
#include "LindaCoordinator.h"

void openChannels(const std::unique_ptr<CommunicationService> &service) {
    const std::string WRITER_COORDINATOR_CHANNEL = "Writer-Coordinator";
    const std::string READER_COORDINATOR_CHANNEL = "Reader-Coordinator";
    const std::string READER_WRITER_CHANNEL = "Reader-Writer";

    bool isChannelCreated = service->openChannel(WRITER_COORDINATOR_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << WRITER_COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }

    isChannelCreated = service->openChannel(READER_COORDINATOR_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << READER_COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }

    isChannelCreated = service->openChannel(READER_WRITER_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << READER_WRITER_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }
}


void closeChannels(const std::unique_ptr<CommunicationService> &service) {
    const std::string WRITER_COORDINATOR_CHANNEL = "Writer-Coordinator";
    const std::string READER_COORDINATOR_CHANNEL = "Reader-Coordinator";
    const std::string READER_WRITER_CHANNEL = "Reader-Writer";

    bool isChannelClosed = service->closeChannel(WRITER_COORDINATOR_CHANNEL);
    if (!isChannelClosed) {
        std::cerr << "Couldn't close FIFO for the " << WRITER_COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
    }

    isChannelClosed = service->closeChannel(READER_COORDINATOR_CHANNEL);
    if (!isChannelClosed) {
        std::cerr << "Couldn't close FIFO for the " << READER_COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
    }

    isChannelClosed = service->closeChannel(READER_WRITER_CHANNEL);
    if (!isChannelClosed) {
        std::cerr << "Couldn't close FIFO for the " << READER_WRITER_CHANNEL
                  << " : " << strerror(errno) << std::endl;
    }
}


int main() {
    const auto &service = std::make_unique<CommunicationService>();
    openChannels(service);

    const auto &coordinator = std::make_unique<LindaCoordinator>(*service);
//    It's assumed that coordinator will read request from the writer then from the reader
    coordinator->handleRequestBlocking();
    coordinator->handleRequestBlocking();

    sleep(1);
    closeChannels(service);
    return 0;
}