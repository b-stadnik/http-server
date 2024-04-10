#include "uart.h"
#include "msg_parser.h"

#include <chrono>
#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>

SerialPort::SerialPort(const std::string& device_path, int baud_rate, size_t buffer_size,
                       std::shared_ptr<DataBase> data_base, std::shared_ptr<InterProcessComm> ip_comm)
    : devicePath_(device_path), baudRate_(baud_rate), bufferSize_(buffer_size), dataBase(std::move(data_base)),
      ipComm(std::move(ip_comm))
{
    dataBuffer.reserve(buffer_size);
    // dataBase->updateConfig(12, true);
}

bool SerialPort::configureUart()
{
    const std::string command = "stty -F " + devicePath_ + " " + std::to_string(baudRate_) + " cs8 -cstopb -parenb";

    int status = system(command.c_str());

    if(status != 0)
    {
        std::cerr << "Error configuring UART settings using stty" << std::endl;
        return false;
    }

    return true;
}

void SerialPort::handleMessage(std::string& msg)
{
    message_parser::MsgCategory msg_category = message_parser::parseMessage(msg);
    // std::cout << "Received from UART: " << msg << " " << int(msg_category) << std::endl;
    if(msg_category == message_parser::MsgCategory::SensorData)
    {
        dataBuffer.push_back(msg);
        std::cout << "size " << dataBuffer.size() << std::endl;

        if(dataBuffer.size() >= bufferSize_)
        {
            flushToDatabase();
        }
    }
    else if(msg_category == message_parser::MsgCategory::Response)
    {
        sendToProcess(msg);
    }
}

void SerialPort::flushToDatabase()
{
    dataBase->storeMultipleMessages(dataBuffer);
    dataBuffer.clear();
}

void SerialPort::sendToProcess(const std::string& data)
{
    ipComm->sendData(data);
}

void SerialPort::run()
{
    std::string received_data;
    std::string ipc_data;
    std::fstream uart_device;

    uart_device.open(devicePath_, std::ios::in | std::ios::out);

    if(!uart_device.is_open())
    {
        std::cerr << "Failed to open UART device" << std::endl;
        return;
    }

    configureUart();

    while(true)
    {
        while(std::getline(uart_device, received_data))
        {
            handleMessage(received_data);
        }

        ipComm->getData(ipc_data);

        // // Send data via UART
        // std::string data_to_send;
        // std::cout << "Enter data to send (or type 'exit' to quit): ";
        // std::getline(std::cin, data_to_send);

        // if (data_to_send == "exit") {
        //     break; // Exit the loop if 'exit' is entered
        // }

        // uart_device << data_to_send << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    uart_device.close();
}
