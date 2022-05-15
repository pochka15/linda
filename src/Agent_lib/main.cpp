#include <iostream>
#include <cstring>
#include <memory>
#include <fstream>
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

void runReader(const char *outputFileName, const char *scenarioFileName) {
    const auto &service = std::make_unique<CommunicationService>();
    const auto &agent = std::make_unique<LindaAgent>(
            "Reader", *service);
    std::string data;

    if (scenarioFileName == nullptr) {
        data = agent->readBlocking("integer:>0, string:”Hello”, float:*");
    } else {
        data = agent->executeScenario(scenarioFileName);
    }

    if (outputFileName == nullptr) {
        std::cout << "Result: reader received tuple: " << data << std::endl;
    } else {
        std::ofstream file(outputFileName);
        file << "Result: reader received tuple: " << data;
        file.close();
    }
}

int main(int argc, char *argv[]) {
    const char *writer = "--writer";
    const char *output = "--output";
    const char *scenario = "--scenario";

    bool isWriter = false;
    char *outputFileName = nullptr, *scenarioFileName = nullptr;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], writer) == 0) {
            isWriter = true;
        }
        else if (strcmp(argv[i], output) == 0) {
            outputFileName = argv[i + 1];
        }
        else if (strcmp(argv[i], scenario) == 0) {
            scenarioFileName = argv[i + 1];
        }
    }

    if (isWriter) runWriter();
    else runReader(outputFileName, scenarioFileName);

    return 0;
}
