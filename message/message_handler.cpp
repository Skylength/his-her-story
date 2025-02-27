#include "message_handler.hpp"
#include "login_message.hpp"
#include "status_message.hpp"
#include <nlohmann/json.hpp>
#include <boost/endian/buffers.hpp>
#include <boost/asio.hpp>

void MessageHandler::handle(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    const std::unique_ptr<Message>& msg
) {
    switch (msg->type()) {
        case 0x0001:  // LoginMessage
            if (auto login_msg = dynamic_cast<const LoginMessage*>(msg.get())) {
                handle_login(socket, *login_msg);
            }
            break;
        case 0x0002:  // StatusMessage
            if (auto status_msg = dynamic_cast<const StatusMessage*>(msg.get())) {
                handle_status(socket, *status_msg);
            }
            break;
        default:
            // 未知消息类型处理
            break;
    }
}

void MessageHandler::handle_login(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    const LoginMessage& msg
) {
    // 示例：登录成功响应
    nlohmann::json response = {
        {"status", "success"},
        {"message", "Welcome " + msg.to_json()["username"].get<std::string>()}
    };
    send_response(socket, response);
}

void MessageHandler::handle_status(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    const StatusMessage& msg
) {
    // 示例：心跳响应
    nlohmann::json response = {
        {"status", "ack"},
        {"received", msg.to_json()["status"].get<std::string>()}
    };
    send_response(socket, response);
}

void MessageHandler::send_response(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    const nlohmann::json& j
) {
    std::string json_str = j.dump();
    uint32_t payload_len = json_str.size();

    // 构造二进制消息（魔数 + 头部 + JSON）
    std::vector<uint8_t> buffer(12 + payload_len);
    *reinterpret_cast<boost::endian::endian_buffer<boost::endian::order::big, uint32_t, 32>*>(buffer.data()) =
        boost::endian::endian_reverse(0xDEADBEEF);
    *reinterpret_cast<boost::endian::endian_buffer<boost::endian::order::big, uint32_t, 32>*>(buffer.data() + 4) =
        boost::endian::endian_reverse(payload_len);
    *reinterpret_cast<boost::endian::endian_buffer<boost::endian::order::big, uint16_t, 16>*>(buffer.data() + 8) =
        boost::endian::endian_reverse(0x0003);  // 假设响应消息类型为0x0003

    std::copy(json_str.begin(), json_str.end(), reinterpret_cast<char*>(buffer.data() + 12));

    // 异步发送响应
    boost::asio::async_write(
        *socket,
        boost::asio::buffer(buffer),
        [socket](const boost::system::error_code&, size_t) {
            // 发送完成后的处理（可选）
        }
    );
}