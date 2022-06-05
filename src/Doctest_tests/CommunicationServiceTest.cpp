#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <functional>
#include <memory>
#include <cstring>
#include <sys/wait.h>
#include "doctest.h"
#include "CommunicationService.h"
#include <thread>

void sendData() {
    const auto &service = std::make_unique<CommunicationService>();
    service->sendBlocking("Hello", "Test");
}

void receiveData() {
    const auto &service = std::make_unique<CommunicationService>();
    const std::string &data = service->receiveBlocking("Test");
}

TEST_CASE("channel is opened and closed without problems") {
    const auto &service = std::make_unique<CommunicationService>();

    bool isChannelCreated = service->openChannel("Test");
    if (!isChannelCreated) {
        FAIL("Couldn't create FIFO: ", strerror(errno));
    }

    bool isChannelClosed = service->closeChannel("Test");
    if (!isChannelClosed) {
        FAIL("Couldn't close FIFO: ", strerror(errno));
    }
}

TEST_CASE("can read and write from channel") {
    const auto &service = std::make_unique<CommunicationService>();

    bool isChannelCreated = service->openChannel("Test");
    if (!isChannelCreated) {
        FAIL("Couldn't create FIFO: ", strerror(errno));
    }

    std::thread(sendData).detach();
    receiveData();

    bool isChannelClosed = service->closeChannel("Test");
    if (!isChannelClosed) {
        FAIL("Couldn't close FIFO: ", strerror(errno));
    }
}


#pragma clang diagnostic pop