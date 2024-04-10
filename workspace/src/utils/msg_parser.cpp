#include "msg_parser.h"

#include <iostream>
#include <sstream>

namespace message_parser
{

MsgCategory parseMessage(std::string& msg)
{
    // Check if the message starts with '$' and ends with '\n'
    if(msg.empty() || msg[0] != '$' || msg.back() != 'n')
    {
        return MsgCategory::Unknown;
    }

    msg = msg.substr(1, msg.size() - 2); // Remove '$' and '\n'
    std::istringstream iss(msg);
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

std::vector<std::string> splitString(const std::string& str)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while(std::getline(iss, token, ','))
    {
        tokens.push_back(token);
    }

    return tokens;
}

} // namespace message_parser
