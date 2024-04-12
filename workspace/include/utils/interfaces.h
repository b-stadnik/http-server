#ifndef UTILS_INTERFACES_H
#define UTILS_INTERFACES_H

#include <string>
#include <vector>

class DataBase
{
  public:
    virtual void storeMessage(const std::string& message) = 0;
    virtual void updateConfig(const std::string& message) = 0;
    virtual void storeMultipleMessages(const std::vector<std::string>& messages) = 0;
    virtual ~DataBase() = default;
};

class InterProcessComm
{
  public:
    virtual void sendData(const std::string& data) = 0;
    virtual void getData(std::string& data) = 0;
    virtual bool connectionOpen() = 0;
    virtual ~InterProcessComm() = default;
};

#endif // UTILS_INTERFACES_H
