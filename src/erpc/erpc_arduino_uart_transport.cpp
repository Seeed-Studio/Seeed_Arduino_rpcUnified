#ifdef WIO_TERMINAL
#include "../../board/wio_terminal/erpc_arduino_uart_transport.cpp"
#elif defined(WIO_LITE_AI)
#include "../../board/wio_lite_ai/erpc_arduino_uart_transport.cpp"
#endif