#include "LindaTupleUtils.h"

std::string joinTupleElements(const Tuple &tuple, const std::string &joinStr) {
    return std::accumulate(
            tuple.begin() + 1,
            tuple.end(),
            formatTupleElement(*tuple.cbegin()),
            [joinStr](const TupleElement &a, const TupleElement &b) {
                return formatTupleElement(a) + joinStr + formatTupleElement(b);
            });
}

std::string formatTuple(const Tuple &tuple) {
    if (tuple.empty()) return "[ ]";
    const std::string &joinedElements = joinTupleElements(tuple, ", ");
    std::stringstream stream;
    stream << "[" << joinedElements << "]";
    return stream.str();
}

std::string formatTupleElement(const TupleElement &element) {
    std::stringstream stream;
    std::visit([&stream](auto &elem) { stream << elem; }, element);
    return stream.str();
}