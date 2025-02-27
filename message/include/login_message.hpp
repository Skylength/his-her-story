#ifndef LOGIN_MESSAGE_HPP
#define LOGIN_MESSAGE_HPP

#include "message.hpp"

class LoginMessage : public Message {
private:
    std::string username;
    std::string password;
public:
    uint16_t type() const override { return 0x0001; }
    nlohmann::json to_json() const override;
    static std::unique_ptr<LoginMessage> from_json(const nlohmann::json&);
};

#endif // LOGIN_MESSAGE_HPP