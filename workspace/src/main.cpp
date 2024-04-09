#include "uart.h"
#include <string>

constexpr auto UART_DEVICE = "/dev/pts/4";

int main()
{

    SerialPort serial_port(UART_DEVICE, 115200, 3);
    serial_port.run();

    return 0;
}