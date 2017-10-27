#include <string.h>
#include <vector>

class Request {
    std::string method;
    std::string path;
    std::string host;

public:
    Request(char* buf) {
        std::vector<std::string> data;
        std::string value = "";
        for (int i = 0; i < strlen(buf); i++) {
            if (buf[i] == ' ') {
                data.push_back(value);
                value = "";
            } else {
                value += buf[i];
            }
        }

        method = data[0];
        path = data[1];
        host = data[4];
    }

    std::string getMethod() {
        return method;
    }

    std::string getPath() {
        return path;
    }

    std::string getHost() {
        return host;
    }
};