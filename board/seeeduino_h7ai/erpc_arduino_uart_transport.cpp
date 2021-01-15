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
  _serial.tx_buff = _tx_buffer;
  _serial.tx_head = 0;
  _serial.tx_tail = 0;
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void EUart::_rx_complete_irq(serial_t *obj)
{
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {

    rx_buffer_index_t i = (unsigned int)(obj->rx_head + 1) % SERIAL_RX_BUFFER_SIZE;

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

// Actual interrupt handlers //////////////////////////////////////////////////////////////

int EUart::_tx_complete_irq(serial_t *obj)
{
  // If interrupts are enabled, there must be more data in the output
  // buffer. Send the next byte
  obj->tx_tail = (obj->tx_tail + 1) % SERIAL_TX_BUFFER_SIZE;

  if (obj->tx_head == obj->tx_tail) {
    return -1;
  }

  return 0;
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
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _serial.rx_head - _serial.rx_tail)) % SERIAL_RX_BUFFER_SIZE;
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
    _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

int EUart::availableForWrite(void)
{
  tx_buffer_index_t head = _serial.tx_head;
  tx_buffer_index_t tail = _serial.tx_tail;

  if (head >= tail) {
    return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
  }
  return tail - head - 1;
}

void EUart::flush()
{
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written) {
    return;
  }

  while ((_serial.tx_head != _serial.tx_tail)) {
    // nop, the interrupt handler will free up space for us
  }
  // If we get here, nothing is queued anymore (DRIE is disabled) and
  // the hardware finished tranmission (TXC is set).
}

size_t EUart::write(uint8_t c)
{
  _written = true;

  tx_buffer_index_t i = (_serial.tx_head + 1) % SERIAL_TX_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  while (i == _serial.tx_tail) {
    // nop, the interrupt handler will free up space for us
  }

  _serial.tx_buff[_serial.tx_head] = c;
  _serial.tx_head = i;

  if (!serial_tx_active(&_serial)) {
    uart_attach_tx_callback(&_serial, _tx_complete_irq);
  }

  return 1;
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
  printf("underlyingReceive: %d\n\r", size);
  while (bytesRead < size)
  {
    printf("bytesRead: %d\n\r", bytesRead);
    while (!m_uartDrv->available()) delay(1);

    const int c = m_uartDrv->read();
    if (c < 0) continue;
    data[bytesRead++] = static_cast<uint8_t>(c);
  }
  for(int i = 0; i < size; i++)
  {
    printf("%02X ", data[i]);
  }

  printf("\n\runderlyingReceive end\n\r");
  return kErpcStatus_Success; // return size != bytesRead ? kErpcStatus_ReceiveFailed : kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
  uint32_t sentSize = 0;
  while (sentSize < size)
  {
    const uint32_t sendSize = min(size - sentSize, 32);
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
