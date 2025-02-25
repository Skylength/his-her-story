#include "AsyncServer.hpp"
#include <iostream>

using boost::asio::ip::tcp;

AsyncServer::AsyncServer(boost::asio::io_context& io, unsigned short port)
    : io_(io), acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void AsyncServer::start() {
    std::cout << "Server started on port " << acceptor_.local_endpoint().port() << std::endl;
    std::cout<<"sdadad"<<std::endl;
}

void AsyncServer::start_accept() {
    auto socket = std::make_shared<tcp::socket>(io_);
    acceptor_.async_accept(*socket, 
        [this, socket](const boost::system::error_code& error) {
            handle_accept(socket, error);
        });
}

void AsyncServer::handle_accept(std::shared_ptr<tcp::socket> socket,
                                const boost::system::error_code& error) {
    if (!error) {
        std::cout << "New connection from " << socket->remote_endpoint() << std::endl;
        start_read(socket);
        start_accept();
    } else {
        std::cerr << "Accept error: " << error.message() << std::endl;
    }
}

void AsyncServer::start_read(std::shared_ptr<tcp::socket> socket) {
    auto buffer = std::make_shared<std::array<char, 1024>>();
    socket->async_read_some(boost::asio::buffer(*buffer),
        [this, socket, buffer](const boost::system::error_code& error, size_t length) {
            handle_read(socket, buffer, error, length);
        });
}

void AsyncServer::handle_read(std::shared_ptr<tcp::socket> socket,
                              std::shared_ptr<std::array<char, 1024>> buffer,
                              const boost::system::error_code& error,
                              size_t length) {
    if (!error) {
        std::cout << "Received from " << socket->remote_endpoint() << ": "
                  << std::string(buffer->data(), length) << std::endl;
        start_read(socket);
    } else if (error != boost::asio::error::eof) {
        std::cerr << "Read error: " << error.message() << std::endl;
    } else {
        std::cout << "Client disconnected: " << socket->remote_endpoint() << std::endl;
    }
}