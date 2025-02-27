#include "status_message.hpp"
#include <nlohmann/json.hpp> // 修正头文件路径

nlohmann::json StatusMessage::to_json() const {
    return {{"status", status}};
}

std::unique_ptr<StatusMessage> StatusMessage::from_json(const nlohmann::json& j) {
    auto msg = std::make_unique<StatusMessage>();
    msg->status = j["status"].get<std::string>();
    return msg;
}