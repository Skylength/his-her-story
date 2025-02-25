#ifndef SERVER_HPP
#define ERVER_HPP

#include <boost/asio.hpp>
#include <memory>


class AsyncServer {
public:
    AsyncServer(boost::asio::io_context& io, unsigned short port);
    void start();

private:
    void start_accept();
    void handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                       const boost::system::error_code& error);
    void start_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void handle_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                     std::shared_ptr<std::array<char, 1024>> buffer,
                     const boost::system::error_code& error,
                     size_t length);

    boost::asio::io_context& io_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif // ASYNC_SERVER_HPP