#include "AsyncServer.hpp"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io;
        std::cout << "Creating server..." << std::endl;
        AsyncServer server(io, 8080);
        std::cout << "Server created." << std::endl;
        server.start();
        std::cout << "Starting event loop..." << std::endl;
        io.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}