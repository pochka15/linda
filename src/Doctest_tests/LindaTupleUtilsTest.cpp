#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "LindaTupleUtils.h"
#include "doctest.h"

TEST_CASE("test if tuple element is formatted correctly") {
    CHECK(formatTupleElement(10) == "10");
}

TEST_CASE("test if tuple is formatted correctly") {
    const Tuple &tuple = Tuple{1, "Hello", 1.34f};
    CHECK(formatTuple(tuple) == "[1, Hello, 1.34]");
}

TEST_CASE("test if empty tuple is formatted correctly") {
    const Tuple &tuple = Tuple();
    CHECK(formatTuple(tuple) == "[ ]");
}

TEST_CASE("test pattern matching 1") {
    const Tuple &tuple = Tuple{1, "Hello", 1.34f};
    const char *pattern = "integer:>0, string:”Hello”, float:*";
    CHECK(checkIfMatches(tuple, pattern));
}

TEST_CASE("test pattern matching 2") {
    const Tuple &tuple = {};
    const char *pattern = "integer:>0, string:”Hello”, float:*";
    CHECK(!checkIfMatches(tuple, pattern));
}

#pragma clang diagnostic pop