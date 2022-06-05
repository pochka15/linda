#ifndef LINDA_STRINGUTILS_H
#define LINDA_STRINGUTILS_H

#include <string>
#include <vector>
#include <sstream>

bool startsWith(const std::string &text, const char *prefix);

std::vector<std::string> readLines(const std::string &data);

#endif //LINDA_STRINGUTILS_H
