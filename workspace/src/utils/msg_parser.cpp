#include "msg_parser.h"

#include <iostream>
#include <sstream>

namespace message_parser
{

constexpr auto SENSOR_DATA_SIZE = 3;
constexpr auto SENSOR_DATA_MIN = 0.0;
constexpr auto SENSOR_DATA_MAX = 1000.0;

MsgCategory parseMessage(std::string& msg)
{
    // Check if the message starts with '$' and ends with '\n'
    if(msg.empty() || msg[0] != '$' || msg.back() != '\n')
    {
        return MsgCategory::Unknown;
    }

    msg = msg.substr(1, msg.size() - 1); // Remove '$' and '\n'
    const auto tokens = splitString(msg, SENSOR_DATA_SIZE);

    if(tokens.size() == SENSOR_DATA_SIZE && tokens.front().find('.') != std::string::npos)
    {
        for(const auto& token : tokens)
        {
            try
            {
                double value = std::stod(token);
                if(value <= SENSOR_DATA_MIN || value >= SENSOR_DATA_MAX)
                {
                    throw std::invalid_argument("Invalid values");
                }
            }
            catch(...)
            {
                return MsgCategory::Unknown;
            }
        }
        return MsgCategory::SensorData;
    }

    try
    {
        int value = std::stoi(tokens.front());
        if(value >= 0)
        {
            return MsgCategory::Response;
        }
    }
    catch(...)
    {
    }

    return MsgCategory::Unknown;
}

std::vector<std::string> splitString(const std::string& str, size_t assumed_size)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    tokens.reserve(assumed_size);
    while(std::getline(iss, token, ','))
    {
        tokens.push_back(token);
    }

    return tokens;
}

} // namespace message_parser
