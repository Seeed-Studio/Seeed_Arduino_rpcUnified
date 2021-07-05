
#ifndef _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_
#define _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_

#include "erpc_framed_transport.h"
#include "Arduino.h"
#include <stdlib.h>

#ifdef WIO_TERMINAL
#include "../../board/wio_terminal/erpc_arduino_uart_transport.h"
#elif defined(WIO_LITE_AI)
#include "../../board/wio_lite_ai/erpc_arduino_uart_transport.h"
#endif

#endif // _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_