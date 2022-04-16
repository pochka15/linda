#include <iostream>
#include <unistd.h>
#include "LindaService.h"

void createChild(const std::string &childName, void (*childRunnable)(), void (*parentRunnable)()) {
    pid_t child = fork();
    bool isChild = child == 0;
    if (isChild) childRunnable();
    else if (child == -1) std::cerr << "Couldn't create " << childName << "! " << strerror(errno) << std::endl;
    else parentRunnable();
}

void runWriter() {
    const auto &lindaServiceP = std::make_unique<LindaService>();
    std::vector<TupleElement> tuple{1, "Hello", 3.14f};
    lindaServiceP->publishTuple(tuple);
}

void runReader() {
}

void runParent() {
    std::cout << "[Parent] PID: " << getpid() << std::endl;
}

void continueParentLogic() {
    createChild("reader", runReader, runParent);
}

int main() {
    createChild("writer", runWriter, continueParentLogic);
    return 0;
}
