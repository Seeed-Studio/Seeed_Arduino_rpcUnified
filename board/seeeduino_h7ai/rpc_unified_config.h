#ifndef PRC_UNIFIED_CONFIG_H
#define PRC_UNIFIED_CONFIG_H

#define RPC_BUFFER_SIZE 4096
#define RPC_BAUD 2000000
#define RPC_SERVER_STACK_SIZE 8192
#define RPC_CLIENT_STACK_SIZE 10240

EUart rpc_uart((void *)USART1, PA10, PB14);

UartTransport g_transport(&rpc_uart, RPC_BAUD);

#define RTL8720_RESET()          \
    do                           \
    {                            \
        SerialUSB.begin(115200); \
        delay(100);              \
        pinMode(PC3, OUTPUT);    \
        digitalWrite(PC3, LOW);  \
        delay(100);              \
        digitalWrite(PC3, HIGH); \
        delay(100);              \
    } while (0)
#endif

#include <stdio.h>
extern "C"
{
    void rpc_printf(const char *format, ...)
    {
        char print_buf[512] = {0};

        va_list args;
        va_start(args, format);
        int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
        va_end(args);

        if (r > 0)
        {
            SerialUSB.write(print_buf);
        }
    }
}
