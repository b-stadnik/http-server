#ifndef INTERFACES_H
#define INTERFACES_H

#include <string>
#include <vector>

class DataBase
{
  public:
    virtual void storeMessage(const std::string& message) = 0;
    virtual void updateConfig(int frequency, bool debug) = 0;
    virtual void storeMultipleMessages(const std::vector<std::string>& messages) = 0;
};

#endif // INTERFACES_H
