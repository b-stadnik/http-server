#include "db_handler.h"
#include "pipes_handler.h"
#include "uart.h"
#include <iostream>

constexpr auto msg_buffer_size = 3;
constexpr auto I_PIPE = "/tmp/pipe";
constexpr auto O_PIPE = "/tmp/pipe2";

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <baudrate> <uart_device> <database_path>\n";
        return 1;
    }

    int baudrate = std::atoi(argv[1]);
    const std::string uart_device = argv[2];
    const std::string db_path = argv[3];

    std::shared_ptr<SqLiteHandler> data_base = std::make_shared<SqLiteHandler>(db_path);
    std::shared_ptr<InterProcessComm> ip_comm = std::make_shared<PipesHandler>(I_PIPE, O_PIPE);

    SerialPort serial_port(uart_device, baudrate, msg_buffer_size, data_base, ip_comm);
    serial_port.run();

    return 0;
}
