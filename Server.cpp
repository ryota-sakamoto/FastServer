#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <map>

#include "Request.cpp"

class Server {
    int sock0;
    sockaddr_in addr;
    // map<string, >
public:
    Server() {
        sock0 = socket(AF_INET, SOCK_STREAM,0);

        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        addr.sin_addr.s_addr = INADDR_ANY;
    }

    void setPort(int port) {
        addr.sin_port = htons(port);
    }

    int getPort() {
        return ntohs(addr.sin_port);
    }

    void setPath(std::string path) {

    }

    void start() {
        std::cout << "Server Start " << ":" << getPort() << std::endl;

        bind(sock0, (struct sockaddr*)&addr, sizeof(addr));
        listen(sock0, 5);

        sockaddr_in client;

        int len = sizeof(client);

        while (true) {
            sockaddr* saddr = (struct sockaddr*)&client;
            int sock = accept(sock0, saddr, (socklen_t*)&len);
            char buf[128];
            read(sock, buf, sizeof(buf));

            char* ip = inet_ntoa(client.sin_addr);

            std::cout << ip << "\n";

            Request* request = new Request(buf);
            std::cout << (*request).getMethod() << std::endl;

            close(sock);
        }

        close(sock0);
    } 
};

