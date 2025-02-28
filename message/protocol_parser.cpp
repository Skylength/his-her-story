#include "protocol_parser.hpp"
#include "login_message.hpp"
#include "status_message.hpp"
#include <boost/endian/conversion.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

std::unique_ptr<Message> ProtocolParser::parse(const uint8_t* data, size_t length) {
    const size_t HEADER_SIZE = 10; // 魔数4 + payload长度4 + 消息类型2 = 10字节

    if (length < HEADER_SIZE) {
        std::cerr << "Error: Message too short (length < " << HEADER_SIZE << ")" << std::endl;
        return nullptr;
    }

    // 解析魔数（大端转本地字节序）
    uint32_t magic = boost::endian::big_to_native(*reinterpret_cast<const uint32_t*>(data));
    std::cout << "[DEBUG] Magic: 0x" << std::hex << magic << std::endl;
    if (magic != 0xDEADBEEF) {
        std::cerr << "Error: Invalid magic number" << std::endl;
        return nullptr;
    }

    // 解析payload长度（大端转本地字节序）
    uint32_t payload_len = boost::endian::big_to_native(*reinterpret_cast<const uint32_t*>(data + 4));
    std::cout << "[DEBUG] Payload length: " << std::dec << payload_len << std::endl;

    // 校验payload长度（可用数据长度 = 总长度 - 头部长度）
    if (payload_len > length - HEADER_SIZE) {
        std::cerr << "Error: Payload length exceeds available data ("
                  << payload_len << " > " << (length - HEADER_SIZE) << ")" << std::endl;
        return nullptr;
    }

    // 解析消息类型（大端转本地字节序）
    uint16_t msg_type = boost::endian::big_to_native(*reinterpret_cast<const uint16_t*>(data + 8));
    std::cout << "[DEBUG] Message type: 0x" << std::hex << msg_type << std::endl;

    // 解析JSON数据（起始位置应为 data + HEADER_SIZE）
    try {
        const char* json_start = reinterpret_cast<const char*>(data + HEADER_SIZE);
        std::string json_str(json_start, payload_len);
        std::cout << "[DEBUG] JSON raw bytes: ";
        for (size_t i = 0; i < payload_len; ++i) {
            printf("%02X ", static_cast<unsigned char>(json_start[i]));
        }
        std::cout << "\n[DEBUG] JSON string: " << json_str << std::endl;

        auto j = nlohmann::json::parse(json_str);
        switch (msg_type) {
            case 0x0001: return LoginMessage::from_json(j);
            case 0x0002: return StatusMessage::from_json(j);
            default:
                std::cerr << "Error: Unknown message type 0x" << std::hex << msg_type << std::endl;
                return nullptr;
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return nullptr;
    }
}