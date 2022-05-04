#include <iostream>
#include "LindaAgent.h"
#include "CommunicationService.h"

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

int main(int argc, char *argv[]) {
    const char *writer = "--writer";
    const char *reader = "--reader";

    if (argc != 2) {
        std::cout << "Please use " << writer << " or " << reader << " options" << '\n';
        return 0;
    }

    char *parameter = argv[1];
    bool isWriter = strcmp(parameter, writer) == 0;
    if (isWriter) runWriter();
    else runReader();

    return 0;
}
