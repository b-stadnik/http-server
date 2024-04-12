#include "pipes_handler.h"

#include <iostream>
#include <stdexcept>

PipesHandler::PipesHandler(const std::string& in_pipe_name, const std::string& out_pipe_name)
    : inPipe(in_pipe_name), outPipe(out_pipe_name)
{
    if(!inPipe.is_open() || !outPipe.is_open())
    {
        throw std::runtime_error("Failed to open named pipe\n");
    }

    std::cout << "Connected to pipes" << std::endl;
}

PipesHandler::~PipesHandler()
{
    inPipe.close();
    outPipe.close();
}

void PipesHandler::sendData(const std::string& data)
{
    outPipe << data << std::endl;
}

void PipesHandler::getData(std::string& data)
{
    std::getline(inPipe, data);
}

bool PipesHandler::connectionOpen()
{
    return inPipe.is_open() && !inPipe.eof() && !inPipe.fail() && outPipe.is_open() && !outPipe.fail();
}
