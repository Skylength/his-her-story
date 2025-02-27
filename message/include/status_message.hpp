#ifndef STATUS_MESSAGE_HPP
#define STATUS_MESSAGE_HPP

#include "message.hpp"

class StatusMessage : public Message {
private:
    std::string status;
public:
    uint16_t type() const override { return 0x0002; }
    nlohmann::json to_json() const override;
    static std::unique_ptr<StatusMessage> from_json(const nlohmann::json&);
};

#endif // STATUS_MESSAGE_HPP