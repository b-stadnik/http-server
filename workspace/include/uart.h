#ifndef UART_H
#define UART_H

#include <fstream>
#include <string>
#include <vector>

class SerialPort
{
  public:
    SerialPort(const std::string& device_path, int baud_rate, size_t buffer_size);
    // void start();
    // void stop();
    void run();

  private:
    bool configureUart();
    void flushToDatabase();
    void handleMessage(const std::string& msg);
    void sendToProcess(const std::string& data);

    std::string devicePath_;
    int baudRate_;
    size_t bufferSize_;
    std::fstream uartDevice_;

    std::vector<std::string> dataBuffer;
};

#endif // UART_H
