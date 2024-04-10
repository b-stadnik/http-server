#include "db_handler.h"
#include "pipes_handler.h"
#include "uart.h"
#include <memory>
#include <string>

constexpr auto UART_DEVICE = "/dev/pts/4";

int main()
{
    std::shared_ptr<SqLiteHandler> data_base = std::make_shared<SqLiteHandler>("database.db");
    std::shared_ptr<InterProcessComm> ip_comm = std::make_shared<PipesHandler>();
    SerialPort serial_port(UART_DEVICE, 115200, 3, data_base, ip_comm);
    serial_port.run();

    return 0;
}