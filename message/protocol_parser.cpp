#include "protocol_parser.hpp"
#include "login_message.hpp"
#include "status_message.hpp"
#include <boost/endian/conversion.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

std::unique_ptr<Message> ProtocolParser::parse(const uint8_t* data, size_t length) {
    if (length < 12) {
        std::cout<<"131";
        return nullptr;
    }

    // 修正后的魔数解析

    // 正确方式 2：若数据存储为大端，转换为本地字节序（推荐）
    auto magic = boost::endian::big_to_native(*reinterpret_cast<const uint32_t*>(data));
    if (magic != 0xDEADBEEF) {
        return nullptr;
    }
    
    // 同理修正其他字段的解析
    auto payload_len = boost::endian::big_to_native(*reinterpret_cast<const uint32_t*>(data + 4));

    if (payload_len > length - 12 || payload_len < 0) {
        return nullptr;
    }


    // 修正后的消息类型解析
    auto msg_type = boost::endian::big_to_native(*reinterpret_cast<const uint16_t*>(data + 8));
    try {
        std::string json_str(reinterpret_cast<const char*>(data + 12), payload_len);
        auto j = nlohmann::json::parse(json_str);

        switch (msg_type) {
            case 0x0001: return LoginMessage::from_json(j);
            case 0x0002: return StatusMessage::from_json(j);
            default: return nullptr;
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return nullptr;
    }
}