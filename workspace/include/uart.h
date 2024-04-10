#ifndef UART_H
#define UART_H

#include "interfaces.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class SerialPort
{
  public:
    SerialPort(const std::string& device_path, int baud_rate, size_t buffer_size, std::shared_ptr<DataBase> data_base);
    // void start();
    // void stop();
    void run();

  private:
    bool configureUart();
    void flushToDatabase();
    void handleMessage(std::string& msg);
    void sendToProcess(const std::string& data);

    std::string devicePath_;
    int baudRate_;
    size_t bufferSize_;
    std::fstream uartDevice_;

    std::vector<std::string> dataBuffer;
    std::shared_ptr<DataBase> dataBase;
};

#endif // UART_H
