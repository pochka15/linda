#include <iostream>
#include <cstring>
#include <memory>
#include <fstream>
#include "LindaAgent.h"
#include "CommunicationService.h"
#include "nlohmann/json.hpp"
#include "RandomUtils.h"

void runWriter(const std::string &id) {
    const auto &service = std::make_unique<CommunicationService>();
    const auto &agent = std::make_unique<LindaAgent>(id, *service);
    std::vector<TupleElement> tuple{1, "Hello", 3.14f};
    agent->publishTupleBlocking(tuple);
    agent->handleRequestBlocking();
}

void runReader(const std::string &outputPath, const std::string &scenarioPath, const std::string &id) {
    const auto &service = std::make_unique<CommunicationService>();
    const auto &agent = std::make_unique<LindaAgent>(id, *service);
    std::string data;

    if (scenarioPath.empty()) {
        data = agent->readBlocking("integer:>0, string:”Hello”, float:*");
    } else {
        const auto &json = nlohmann::json::parse(std::ifstream(scenarioPath));
        data = agent->executeScenario(json);
    }

    if (outputPath.empty()) {
        std::cout << data << std::endl;
    } else {
        std::ofstream file(outputPath);
        file << data;
        file.close();
    }
}

int main(int argc, char *argv[]) {
    const char *writer = "--writer";
    const char *output = "--output";
    const char *scenario = "--scenario";

    bool isWriter = false;
    std::string outputFileName;
    std::string scenarioFileName;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], writer) == 0) {
            isWriter = true;
        } else if (strcmp(argv[i], output) == 0) {
            outputFileName = argv[i + 1];
        } else if (strcmp(argv[i], scenario) == 0) {
            scenarioFileName = argv[i + 1];
        }
    }

    if (isWriter) runWriter(std::string("Writer ") + getRandomId());
    else runReader(outputFileName, scenarioFileName, "Reader " + getRandomId());

    return 0;
}
