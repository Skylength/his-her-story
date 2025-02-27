#include "cstdint"
#include "memory"
#include "string"
#include "vector"
#include <nlohmann/json.hpp> // 修正头文件路径
using json = nlohmann::json;

class Message {
    public:
        virtual ~Message() = default;
        virtual uint16_t type() const = 0;
        virtual json to_json() const = 0; // 返回JSON对象
        static std::unique_ptr<Message> from_json(uint16_t type, const json& j); // 工厂方法
};

