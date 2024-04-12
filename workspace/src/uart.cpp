#include "uart.h"
#include "msg_parser.h"

#include <chrono>
#include <iostream>
#include <thread>

constexpr auto CONFIG_MSG_ID = '2';

SerialPort::SerialPort(const std::string& device_path, int baud_rate, size_t buffer_size,
                       std::shared_ptr<DataBase> data_base, std::shared_ptr<InterProcessComm> ip_comm)
    : devicePath_(device_path), baudRate_(baud_rate), msgBufferSize_(buffer_size), dataBase(std::move(data_base)),
      ipComm(std::move(ip_comm))
{
    dataBuffer.reserve(buffer_size);
}

bool SerialPort::configureUart(boost::asio::serial_port& uart_device)
{
    uart_device.set_option(boost::asio::serial_port_base::baud_rate(115200));
    uart_device.set_option(boost::asio::serial_port_base::character_size(8 /* data bits */));
    uart_device.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    uart_device.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    return true;
}

void SerialPort::handleMessage(std::string& msg)
{
    message_parser::MsgCategory msg_category = message_parser::parseMessage(msg);
    // std::cout << "Received from UART: " << msg << " " << int(msg_category) << std::endl;
    if(msg_category == message_parser::MsgCategory::SensorData)
    {
        dataBuffer.push_back(msg);
        std::cout << "Received UART msgs buffer size " << dataBuffer.size() << std::endl;

        if(dataBuffer.size() >= msgBufferSize_)
        {
            flushToDatabase();
        }
    }
    else if(msg_category == message_parser::MsgCategory::Response)
    {
        if(msg.front() == CONFIG_MSG_ID)
        {
            dataBase->updateConfig(msg);
            std::cout << "sent to database" << std::endl;

        }
        msg = "$" + msg + "\n";
        std::cout << "sent to process " << msg << std::endl;
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
    // std::cout << data << std::endl;
}

void SerialPort::getFromProcess(std::string& data)
{
    ipComm->getData(data);
    // std::cout << data << std::endl;
}

void SerialPort::run()
{
    std::string ipc_data;
    boost::asio::io_service io;
    boost::asio::serial_port uart_device(io, devicePath_);

    if(!uart_device.is_open())
    {
        std::cerr << "Failed to open UART device" << std::endl;
        return;
    }

    std::cout << "Connected to UART device" << std::endl;

    configureUart(uart_device);

    std::thread uart_reader_thread([&]() {
        std::array<char, 256> data;

        while(true)
        {
            // Reading data from uart
            size_t n_read = uart_device.read_some(boost::asio::buffer(data, data.size()));
            std::string received_data(data.data(), n_read);

            handleMessage(received_data);

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    std::thread ip_comm_thread([&]() {
        while(ipComm->connectionOpen())
        {
            getFromProcess(ipc_data);
            sendToProcess(ipc_data);
            // Writing data to uart
            boost::asio::write(uart_device, boost::asio::buffer(ipc_data.c_str(), ipc_data.size()));
        }
        std::cout << "IPC connection closed" << std::endl;
    });

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    uart_reader_thread.join();
    ip_comm_thread.join();

    uart_device.close();
}
