#ifndef LINDA_LINDASERVICE_H
#define LINDA_LINDASERVICE_H

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>
#include "LindaTupleUtils.h"


class LindaService {
public:
    void publishTuple(Tuple &tuple);
};

#endif //LINDA_LINDASERVICE_H
