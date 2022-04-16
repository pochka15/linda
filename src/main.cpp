#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid = fork();

//    Child
    if (pid == 0) {
        std::cout << "[Child] PID: " << getpid() << std::endl;
    }

//    Parent
    else if (pid > 0) {
        std::cout << "[Parent] PID: " << getpid() << std::endl;
    }

//    Error
    else {
        std::cout << "Error" << std::endl;
    }
    return 0;
}
