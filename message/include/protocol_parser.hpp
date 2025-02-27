#pragma once

#include <memory>
#include <vector>
#include <boost/endian/buffers.hpp>
#include "message.hpp"


class ProtocolParser : public Message{
public:
    static std::unique_ptr<Message> parse(const uint8_t*, size_t);
};