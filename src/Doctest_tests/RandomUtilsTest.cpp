#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "doctest.h"
#include "RandomUtils.h"


TEST_CASE("random ids are different") {
    CHECK(getRandomId() != getRandomId());
}

#pragma clang diagnostic pop