#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <functional>

#include "Request.cpp"

class Server {
    int sock0;
    sockaddr_in addr;
    std::map<std::string, std::function<void(Request*)>> _route;
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

    void route(std::string path, std::function<void(Request*)> func) {
        _route[path] = func;
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
            if (_route.count(request->getPath()) == 1) {
                std::function<void(Request*)> func = _route[request->getPath()];
                func(request);
                send_msg(sock, (char*)"HTTP/1.1 200 OK\r\n");
            } else {
                std::cout << "404\n";
                send_msg(sock, (char*)"HTTP/1.1 404 Not Found\r\n");
            }
            send_msg(sock, (char*)"Content-Length: 0\r\n");
            send_msg(sock, (char*)"Content-Type: text/html\r\n");
            send_msg(sock, (char*)"Connection: Close\r\n");
            send_msg(sock, (char*)"\r\n");

            delete request;
            close(sock);
        }

        close(sock0);
    }

    void send_msg(int sock, char* message) {
        int len = strlen(message);
        write(sock, message, len);
    }
};

