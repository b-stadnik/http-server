#ifndef PIPES_HANDLER_H
#define PIPES_HANDLER_H

#include "interfaces.h"
#include <fstream>

class PipesHandler : public InterProcessComm
{
  public:
    PipesHandler(const std::string& in_pipe_name, const std::string& out_pipe_name);
    ~PipesHandler();
    void sendData(const std::string& data) override;
    void getData(std::string& data) override;
    bool connectionOpen() override;

  private:
    std::ifstream inPipe;
    std::ofstream outPipe;
};

#endif // PIPES_HANDLER_H
