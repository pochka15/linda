#include <iostream>
#include <unistd.h>
#include "LindaCoordinator.h"
#include "LindaAgent.h"
#include "CommunicationService.h"

void openChannels(const std::unique_ptr<CommunicationService> &service);

void closeChannels(const std::unique_ptr<CommunicationService> &service);

void createChild(const std::string &childName,
                 const std::function<void(void)> &childRunnable,
                 const std::function<void(void)> &parentRunnable) {
    pid_t child = fork();
    bool isChild = child == 0;
    if (isChild) childRunnable();
    else if (child == -1) std::cerr << "Couldn't create " << childName << "! " << strerror(errno) << std::endl;
    else parentRunnable();
}

void runWriter() {
    const auto &service = std::make_unique<CommunicationService>();
    const auto &agent = std::make_unique<LindaAgent>(
            "Writer", *service);
    std::vector<TupleElement> tuple{1, "Hello", 3.14f};
    agent->publishTupleBlocking(tuple);
    agent->handleRequestBlocking();
}

void runReader() {
    const auto &service = std::make_unique<CommunicationService>();
    const auto &agent = std::make_unique<LindaAgent>(
            "Reader", *service);
    const std::string &data = agent->readBlocking("integer:>0, string:”Hello”, float:*");
    std::cout << "Result: reader received pattern: " << data << std::endl;
}

void runCoordinator() {
    const auto &service = std::make_unique<CommunicationService>();

    const auto &coordinator = std::make_unique<LindaCoordinator>(*service);
//    It's assumed that coordinator will read request from the writer then from the reader
    coordinator->handleRequestBlocking();
    coordinator->handleRequestBlocking();

//    Wait for each child
    int status;
    while (wait(&status) > 0);
    closeChannels(service);
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

void openChannels(const std::unique_ptr<CommunicationService> &service) {
    const std::string WRITER_COORDINATOR_CHANNEL = "Writer-Coordinator";
    const std::string READER_COORDINATOR_CHANNEL = "Reader-Coordinator";
    const std::string READER_WRITER_CHANNEL = "Reader-Writer";

    bool isChannelCreated = service->createChannel(WRITER_COORDINATOR_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << WRITER_COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }

    isChannelCreated = service->createChannel(READER_COORDINATOR_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << READER_COORDINATOR_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }

    isChannelCreated = service->createChannel(READER_WRITER_CHANNEL);
    if (!isChannelCreated) {
        std::cerr << "Couldn't create FIFO for the " << READER_WRITER_CHANNEL
                  << " : " << strerror(errno) << std::endl;
        return;
    }
}


int main() {
    openChannels(std::make_unique<CommunicationService>());
    createChild("writer", runWriter, []() {
        createChild("reader", runReader, runCoordinator);
    });
    return 0;
}
