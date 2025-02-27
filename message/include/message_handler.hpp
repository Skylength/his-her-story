#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include "message.hpp"      // 包含消息基类
#include "login_message.hpp"
#include "status_message.hpp"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp> // 修正头文件路径
#include <memory>

class MessageHandler {
public:
    // 处理消息的入口函数
    void handle(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::unique_ptr<Message>& msg);

private:
    // 具体消息类型的处理函数（私有实现）
    void handle_login(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const LoginMessage& msg);
    void handle_status(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const StatusMessage& msg);

    // 响应构造函数（示例）
    void send_response(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const nlohmann::json& j);
};

#endif // MESSAGE_HANDLER_HPP