#ifndef PRC_UNIFIED_CONFIG_H
#define PRC_UNIFIED_CONFIG_H

#ifdef WIO_TERMINAL

#define RPC_BUFFER_SIZE 4096
#define RPC_BAUD 1843200
#define RPC_SERVER_STACK_SIZE 8192
#define RPC_CLIENT_STACK_SIZE 20480

#define PIN_BLE_SERIAL_X_RX (84ul)
#define PIN_BLE_SERIAL_X_TX (85ul)
#define PAD_BLE_SERIAL_X_RX (SERCOM_RX_PAD_2)
#define PAD_BLE_SERIAL_X_TX (UART_TX_PAD_0)
#define SERCOM_BLE_SERIAL_X sercom0
#define INTERRUPT_HANDLER_IMPLEMENT_BLE_SERIAL_X(uart) \
    void SERCOM0_0_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }                                                  \
    void SERCOM0_1_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }                                                  \
    void SERCOM0_2_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }                                                  \
    void SERCOM0_3_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }

EUart rpc_uart(&SERCOM_BLE_SERIAL_X, PIN_BLE_SERIAL_X_RX, PIN_BLE_SERIAL_X_TX, PAD_BLE_SERIAL_X_RX, PAD_BLE_SERIAL_X_TX);
extern "C"
{
    INTERRUPT_HANDLER_IMPLEMENT_BLE_SERIAL_X(rpc_uart)
}

UartTransport g_transport(&rpc_uart, RPC_BAUD);

#define RTL8720_RESET()                       \
    do                                        \
    {                                         \
        pinMode(RTL8720D_CHIP_PU, OUTPUT);    \
        digitalWrite(RTL8720D_CHIP_PU, LOW);  \
        delay(100);                           \
        digitalWrite(RTL8720D_CHIP_PU, HIGH); \
        delay(100);                           \
    } while (0)

#else

#error "This board does not yet support!"

#endif

#endif
