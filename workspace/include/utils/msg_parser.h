#ifndef UTILS_MSG_PARSER_H
#define UTILS_MSG_PARSER_H

#include <string>
#include <vector>

namespace message_parser
{

enum class MsgCategory
{
    SensorData,
    Response,
    Unknown
};

MsgCategory parseMessage(std::string& msg);
std::vector<std::string> splitString(const std::string& str, size_t assumed_size);

} // namespace message_parser

#endif // UTILS_MSG_PARSER_H
