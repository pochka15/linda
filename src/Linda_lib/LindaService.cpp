#include "LindaService.h"

void LindaService::publishTuple(Tuple &tuple) {
    std::cout << "Publish: " << formatTuple(tuple) << std::endl;
}


