#include "StringUtils.h"

bool startsWith(const std::string &text, const char *prefix) {
    return text.rfind(prefix, 0) == 0;
}

std::vector<std::string> readLines(const std::string &data) {
    std::vector<std::string> lines;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) lines.push_back(line);
    return lines;
}

