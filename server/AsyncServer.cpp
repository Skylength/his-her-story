#include "AsyncServer.hpp"
#include "protocol_parser.hpp" // 引入协议解析模块
#include "message_handler.hpp" // 引入消息处理模块
#include <iostream>

using boost::asio::ip::tcp;

AsyncServer::AsyncServer(boost::asio::io_context& io, unsigned short port)
    : io_(io), acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
    try {
        std::cout << "AsyncServer constructor called." << std::endl;
        start_accept();
    } catch (const boost::system::system_error& e) {
        std::cerr << "Acceptor initialization failed: " << e.what() << std::endl;
        throw; // 重新抛出异常，避免静默失败
    }
}

void AsyncServer::start() {
    std::cout << "Server started on port " << acceptor_.local_endpoint().port() << std::endl;
}

void AsyncServer::start_accept() {
    std::cout << "Waiting for new connections..." << std::endl;
    auto socket = std::make_shared<tcp::socket>(io_);
    acceptor_.async_accept(*socket, 
        [this, socket](const boost::system::error_code& error) {
            handle_accept(socket, error);
        });
}

void AsyncServer::handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                                const boost::system::error_code &error)
{
    if (!error)
    {
        std::cout << "New connection from " << socket->remote_endpoint() << std::endl;
        start_read(socket);
        start_accept();
    }
    else
    {
        std::cerr << "Accept error: " << error.message() << std::endl;
    }
}

void AsyncServer::start_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    std::cout << "Waiting for data from " << socket->remote_endpoint() << std::endl;
    auto buffer = std::make_shared<std::array<char, 1024>>();
    socket->async_read_some(boost::asio::buffer(*buffer),
        [this, socket, buffer](const boost::system::error_code& error, size_t length) {
            handle_read(socket, buffer, error, length);
        });
}

void AsyncServer::handle_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                              std::shared_ptr<std::array<char, 1024>> buffer,
                              const boost::system::error_code &error,
                              size_t length)
{
    std::cout << "Received " << length << " bytes from " << socket->remote_endpoint() << std::endl;
    if (!error)
    {
        std::cout << "Processing received data..." << std::endl;
        std::cout << "Raw data: ";
        for (size_t i = 0; i < length; ++i) {
            printf("%02X ", static_cast<unsigned char>(buffer->data()[i]));
        }
        // 使用协议解析模块处理数据
        auto msg = ProtocolParser::parse(reinterpret_cast<const uint8_t*>(buffer->data()), length);
        if (msg)
        {
            std::cout << "Message parsed successfully." << std::endl;
            MessageHandler handler;
            handler.handle(socket, msg); // 调用消息处理器
        }
        else
        {
            std::cerr << "Failed to parse message." << std::endl;
        }
        start_read(socket);
    }
    else if (error != boost::asio::error::eof)
    {
        std::cerr << "Read error: " << error.message() << std::endl;
    }
    else
    {
        std::cout << "Client disconnected: " << socket->remote_endpoint() << std::endl;
    }
}