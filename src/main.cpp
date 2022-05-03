#include <iostream>
#include <unistd.h>
#include "Coordinator_lib/LindaCoordinator.h"
#include "Agent_lib/LindaAgent.h"
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

int main() {
    createChild("writer", runWriter, runReader);
    return 0;
}
