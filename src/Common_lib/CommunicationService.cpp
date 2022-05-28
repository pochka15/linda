#include "CommunicationService.h"
#include <cstring>

static const int BUFFER_SIZE = 1024;

void CommunicationService::sendBlocking(const std::string &data, const std::string &channel) const {
    std::string path = FIFO_DIRECTORY + "/" + channel;

    int descriptor = open(path.c_str(), O_WRONLY);
    if (descriptor == -1) {
        std::cerr << "Couldn't open a FIFO for writing at " << path << ", error:" << strerror(errno) << "\n";
        return;
    }

    write(descriptor, data.c_str(), BUFFER_SIZE);
    close(descriptor);
}

std::string CommunicationService::receiveBlocking(const std::string &channel) const {
    std::string path = FIFO_DIRECTORY + "/" + channel;

    int descriptor = open(path.c_str(), O_RDONLY);
    if (descriptor == -1) {
        std::cerr << "Couldn't open a FIFO for reading at " << path << ", error:" << strerror(errno) << "\n";
        return "";
    }

    char buffer[BUFFER_SIZE];
    read(descriptor, buffer, BUFFER_SIZE);
    close(descriptor);
    buffer[BUFFER_SIZE - 1] = '\0'; // ensure that string will contain maximum BUFFER_SIZE chars
    return {buffer};
}

bool CommunicationService::openChannel(const std::string &channel) const {
    std::string path = FIFO_DIRECTORY + "/" + channel;
    int fifoResult = mkfifo(path.c_str(), S_IRUSR | S_IWUSR);
    return fifoResult != -1;
}

bool CommunicationService::closeChannel(const std::string &channel) const {
    std::string path = FIFO_DIRECTORY + "/" + channel;
    return unlink(path.c_str()) != -1;
}