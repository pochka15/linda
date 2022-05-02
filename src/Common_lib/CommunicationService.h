#ifndef LINDA_COMMUNICATIONSERVICE_H
#define LINDA_COMMUNICATIONSERVICE_H


#include <string>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

class CommunicationService {
public:
    void sendBlocking(const std::string &data, const std::string &channel) const;

    [[nodiscard]] std::string receiveBlocking(const std::string &channel) const;

    [[nodiscard]] bool createChannel(const std::string &channel) const;

    [[nodiscard]] bool closeChannel(const std::string &channel) const;

private:
    const std::string FIFO_DIRECTORY = "/tmp";
};


#endif //LINDA_COMMUNICATIONSERVICE_H
