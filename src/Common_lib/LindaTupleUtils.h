#ifndef LINDA_LINDATUPLEUTILS_H
#define LINDA_LINDATUPLEUTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>

using TupleElement = std::variant<int, std::string, float>;
using Tuple = std::vector<TupleElement>;

std::string formatTupleElement(const TupleElement &element);
std::string formatTuple(const Tuple &tuple);

bool checkIfMatches(const Tuple &tuple, const std::string &pattern);

#endif //LINDA_LINDATUPLEUTILS_H
