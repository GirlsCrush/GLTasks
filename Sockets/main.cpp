#include <iostream>
#include <cstring>
#include <memory>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef int SOCKET;

const char *PORT = "80";
const char *ADDRESS = "91.202.128.77";
const int bufSize = 128;

int main() {
    
    SOCKET s;

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == 0) {
        std::cout << "Failed to create socket!" << std::endl;
        close(s);
        return -1;
    }

    addrinfo *dest_addr;

    if (0 != getaddrinfo(ADDRESS, PORT, nullptr, &dest_addr)) {
        std::cout << "getaddrinfo failed!" << std::endl;
        return -1;
    }

    std::cout << "Socket address is ok!" << std::endl;

    if (connect(s, dest_addr->ai_addr, dest_addr->ai_addrlen) != 0) {
        std::cout << "Connection failed!" << std::endl;
        return -1;
    }

    std::cout << "Connect is ok!" << std::endl;

    std::string command = "GET /\r\n\r\n";

    int res = send(s, command.c_str(), command.length(), 0);
    if (res != command.length()) {
        std::cout << "Send failed! Sent " << res << " bytes of " << command.length() << std::endl;
        return -1;
    }

    std::cout << "Sent is ok!" << std::endl;

    auto buf = std::make_unique<char[]>(bufSize);
    memset(buf.get(), 0, bufSize);

    std::string data;
    while (recv(s, buf.get(), bufSize - 1, 0) > 0) {
        data += buf.get();
        memset(buf.get(), 0, bufSize);
    }

    std::cout << data << std::endl;
    return 0;
}