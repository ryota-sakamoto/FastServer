#include "Server.cpp"

void testFunc(Request*);

int main() {
    Server server;
    server.route("/", testFunc);
    server.start();
}

void testFunc(Request* request) {
    std::cout << "testFunc Call\n";
    std::cout << "method: " << request->getMethod() << "\n";
}