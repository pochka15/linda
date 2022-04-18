#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "doctest.h"
#include "CommunicationService.h"

void createChild(const std::string &childName,
                 const std::function<void(void)> &childRunnable,
                 const std::function<void(void)> &parentRunnable) {
    pid_t child = fork();
    bool isChild = child == 0;
    if (isChild) childRunnable();
    else if (child == -1) std::cerr << "Couldn't create " << childName << "! " << strerror(errno) << std::endl;
    else parentRunnable();
}

void sendData() {
    const auto &service = std::make_unique<CommunicationService>();
    service->sendBlocking("Hello", "Test");
}

void receiveData() {
    const auto &service = std::make_unique<CommunicationService>();
    const std::string &data = service->receiveBlocking("Test");
}

TEST_CASE("listeningChannel is opened and closed without problems") {
    const auto &service = std::make_unique<CommunicationService>();

    bool isChannelCreated = service->createChannel("Test");
    if (!isChannelCreated) {
        FAIL("Couldn't create FIFO: ", strerror(errno));
    }

    bool isChannelClosed = service->closeChannel("Test");
    if (!isChannelClosed) {
        FAIL("Couldn't close FIFO: ", strerror(errno));
    }
}

TEST_CASE("can read and write from listeningChannel") {
    const auto &service = std::make_unique<CommunicationService>();

    bool isChannelCreated = service->createChannel("Test");
    if (!isChannelCreated) {
        FAIL("Couldn't create FIFO: ", strerror(errno));
    }

    createChild("writer", sendData, [&service]() {
        createChild("reader", receiveData, [&service]() {
//            Wait for each child
            int status;
            while (wait(&status) > 0);
            bool isChannelClosed = service->closeChannel("Test");
            if (!isChannelClosed) {
                FAIL("Couldn't close FIFO: ", strerror(errno));
            }
        });
    });
}


#pragma clang diagnostic pop