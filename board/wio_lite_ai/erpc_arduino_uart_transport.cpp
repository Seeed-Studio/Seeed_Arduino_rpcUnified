/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../../src/erpc/erpc_arduino_uart_transport.h"
#include "Arduino.h"
#include "wiring_private.h"

using namespace erpc;
// Constructors ////////////////////////////////////////////////////////////////
EUart::EUart(void *peripheral, uint32_t _rx, uint32_t _tx)
{
  _serial.uart = (USART_TypeDef*)peripheral;
  init(digitalPinToPinName(_rx), digitalPinToPinName(_tx));
}

EUart::EUart(void *peripheral, PinName _rx, PinName _tx)
{
  _serial.uart = (USART_TypeDef*)peripheral;
  _serial.pin_rx = _rx;
  _serial.pin_tx = _tx;
  init(_serial.pin_rx, _serial.pin_tx);
}

void EUart::init(PinName _rx, PinName _tx)
{
  if (_rx == _tx) {
    _serial.pin_rx = NC;
  } else {
    _serial.pin_rx = _rx;
  }
  _serial.pin_tx = _tx;
  _serial.rx_buff = _rx_buffer;
  _serial.rx_head = 0;
  _serial.rx_tail = 0;
  _serial.tx_buff = NULL;
  _serial.tx_head = 0;
  _serial.tx_tail = 0;
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void EUart::_rx_complete_irq(serial_t *obj)
{
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {

    uint32_t i = (unsigned int)(obj->rx_head + 1) % RPC_UART_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != obj->rx_tail) {
      obj->rx_buff[obj->rx_head] = c;
      obj->rx_head = i;
    }
  }
}

// Public Methods //////////////////////////////////////////////////////////////

void EUart::begin(unsigned long baud, byte config)
{
  uint32_t databits = 0;
  uint32_t stopbits = 0;
  uint32_t parity = 0;

  _baud = baud;
  _config = config;

  // Manage databits
  switch (config & 0x07) {
    case 0x02:
      databits = 6;
      break;
    case 0x04:
      databits = 7;
      break;
    case 0x06:
      databits = 8;
      break;
    default:
      databits = 0;
      break;
  }

  if ((config & 0x30) == 0x30) {
    parity = UART_PARITY_ODD;
    databits++;
  } else if ((config & 0x20) == 0x20) {
    parity = UART_PARITY_EVEN;
    databits++;
  } else {
    parity = UART_PARITY_NONE;
  }

  if ((config & 0x08) == 0x08) {
    stopbits = UART_STOPBITS_2;
  } else {
    stopbits = UART_STOPBITS_1;
  }

  switch (databits) {
#ifdef UART_WORDLENGTH_7B
    case 7:
      databits = UART_WORDLENGTH_7B;
      break;
#endif
    case 8:
      databits = UART_WORDLENGTH_8B;
      break;
    case 9:
      databits = UART_WORDLENGTH_9B;
      break;
    default:
    case 0:
      Error_Handler();
      break;
  }

  uart_init(&_serial, (uint32_t)baud, databits, parity, stopbits);
  uart_attach_rx_callback(&_serial, _rx_complete_irq);

  if (HAL_UARTEx_SetTxFifoThreshold(&(_serial.handle), UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&(_serial.handle), UART_RXFIFO_THRESHOLD_8_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableFifoMode(&(_serial.handle)) != HAL_OK)
  {
    Error_Handler();
  }

}

void EUart::end()
{
  // wait for transmission of outgoing data
  flush();

  uart_deinit(&_serial);

  // clear any received data
  _serial.rx_head = _serial.rx_tail;
}

int EUart::available(void)
{
  return ((unsigned int)(RPC_UART_RX_BUFFER_SIZE + _serial.rx_head - _serial.rx_tail)) % RPC_UART_RX_BUFFER_SIZE;
}

int EUart::peek(void)
{
  if (_serial.rx_head == _serial.rx_tail) {
    return -1;
  } else {
    return _serial.rx_buff[_serial.rx_tail];
  }
}

int EUart::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (_serial.rx_head == _serial.rx_tail) {
    return -1;
  } else {
    unsigned char c = _serial.rx_buff[_serial.rx_tail];
    _serial.rx_tail = (uint32_t)(_serial.rx_tail + 1) % RPC_UART_RX_BUFFER_SIZE;
    return c;
  }
}

void EUart::flush()
{
}

size_t EUart::write(const uint8_t *buffer, size_t size)
{
  return uart_write(&_serial, (uint8_t *)buffer, size);
}

size_t EUart::write(uint8_t c)
{
  uint8_t buff = c;
  return write(&buff, 1);
}
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

UartTransport::UartTransport(EUart *uartDrv, unsigned long baudrate)
    : m_uartDrv(uartDrv), m_baudrate(baudrate)
{
}

UartTransport::~UartTransport(void)
{
}

erpc_status_t UartTransport::init(void)
{

  m_uartDrv->begin(m_baudrate);
  return kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
  uint32_t bytesRead = 0;
  while (bytesRead < size)
  {
    while (!m_uartDrv->available()) delay(1);

    const int c = m_uartDrv->read();
    if (c < 0) continue;
    data[bytesRead++] = static_cast<uint8_t>(c);
  }
  return kErpcStatus_Success; // return size != bytesRead ? kErpcStatus_ReceiveFailed : kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
  uint32_t sentSize = 0;
  while (sentSize < size)
  {
    const uint32_t sendSize =min(size - sentSize, 32);
    sentSize += m_uartDrv->write(&data[sentSize], sendSize);
    delay(4);
  }
  return kErpcStatus_Success; // return size != offset ? kErpcStatus_SendFailed : kErpcStatus_Success;
}

bool UartTransport::hasMessage()
{
  if (m_uartDrv->available())
  {
    return true;
  }
  return false;
}
