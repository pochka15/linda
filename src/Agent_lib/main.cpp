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

void runReader(bool isOutput, char* outputFileName, bool isScenario, char* scenarioFileName) {
    const auto &service = std::make_unique<CommunicationService>();
    const auto &agent = std::make_unique<LindaAgent>(
            "Reader", *service);

    if (isOutput && isScenario) {
        const std::string &data = agent->executeScenario(scenarioFileName);

        std::ofstream File(outputFileName);
        File << data;
        File.close();
        return;
    }

    const std::string &data = agent->readBlocking("integer:>0, string:”Hello”, float:*");
    std::cout << "Result: reader received pattern: " << data << std::endl;
}

int main(int argc, char *argv[]) {
    const char *writer = "--writer";
    const char *reader = "--reader";
    const char *output = "--output";
    const char *scenario = "--scenario";

    bool isWriter = false, isOutput = false, isScenario = false;
    char *outputFileName, *scenarioFileName;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], writer) == 0) {
            isWriter = true;
        }
        else if (strcmp(argv[i], output) == 0) {
            isOutput = true;
            outputFileName = argv[i + 1];
        }
        else if (strcmp(argv[i], scenario) == 0) {
            isScenario = true;
            scenarioFileName = argv[i + 1];
        }
    }

    if (isWriter) runWriter();
    else runReader(isOutput, outputFileName, isScenario, scenarioFileName);

    return 0;
}
