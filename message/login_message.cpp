#include "login_message.hpp"
#include <nlohmann/json.hpp> // 修正头文件路径

nlohmann::json LoginMessage::to_json() const {
    return {{"username", username}, {"password", password}};
}
// login_message.cpp
std::unique_ptr<LoginMessage> LoginMessage::from_json(const nlohmann::json& j) {
    auto msg = std::make_unique<LoginMessage>();
    msg->username = j["user"].get<std::string>();
    msg->password = j["pass"].get<std::string>();
    return msg;
}