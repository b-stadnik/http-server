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
    const auto tokens = splitString(msg);

    if(tokens.size() == 3 && tokens.front().find('.') != std::string::npos)
    {
        for(const auto& token : tokens)
        {
            try
            {
                float value = std::stof(token);
                if(value <= 0.0 || value >= 1000.0)
                {
                    throw;
                }
            }
            catch(...)
            {
                return MsgCategory::Unknown;
            }
        }
        return MsgCategory::SensorData;
    }
    else
    {
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
