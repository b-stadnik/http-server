#include "db_handler.h"
#include "pipes_handler.h"
#include "uart.h"

constexpr auto UART_DEVICE = "/tmp/serial_mock1";

int main()
{
    std::shared_ptr<SqLiteHandler> data_base = std::make_shared<SqLiteHandler>("database.db");
    std::shared_ptr<InterProcessComm> ip_comm = std::make_shared<PipesHandler>("/tmp/pipe", "/tmp/pipe2");

    SerialPort serial_port(UART_DEVICE, 115200, 3, data_base, ip_comm);
    serial_port.run();

    return 0;
}
