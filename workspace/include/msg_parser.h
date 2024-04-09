#ifndef MSG_PARSER_H
#define MSG_PARSER_H

#include <iostream>
#include <sstream>
#include <string>

namespace message_parser
{

enum class MsgCategory
{
    SensorData,
    Response,
    Unknown
};

MsgCategory parseMessage(const std::string& msg)
{
    // Check if the message starts with '$' and ends with '\n'
    if(msg.empty() || msg[0] != '$' || msg.back() != 'n')
    {
        return MsgCategory::Unknown;
    }

    std::istringstream iss(msg.substr(1, msg.size() - 2)); // Remove '$' and '\n'
    std::string token;
    std::getline(iss, token, ',');

    // Assuming first token is float for SensorData, or int for Response
    if(token.find('.') != std::string::npos)
    {
        try
        {
            float value = std::stof(token);
            if(value >= 0.0 && value <= 1000.0)
            {
                return MsgCategory::SensorData;
            }
        }
        catch(...)
        {
        }
    }
    else
    {
        try
        {
            int value = std::stoi(token);
            if(value >= 0)
            {
                return MsgCategory::Response;
            }
        }
        catch(...)
        {
        }
    }

    return MsgCategory::Unknown;
}

} // namespace message_parser

#endif // MSG_PARSER_H
