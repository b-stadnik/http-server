#ifndef PIPES_HANDLER_H
#define PIPES_HANDLER_H

#include "interfaces.h"

class PipesHandler : public InterProcessComm
{
  public:
    void sendData(const std::string& data) override;
    void getData(std::string& data) override;
};

#endif // PIPES_HANDLER_H
