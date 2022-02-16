/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_arduino_uart_transport.h"
#include "Arduino.h"
#include "wiring_private.h"

using namespace erpc;

#define NO_RTS_PIN 255
#define NO_CTS_PIN 255
#define RTS_RX_THRESHOLD 10

EUart::EUart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX) : EUart(_s, _pinRX, _pinTX, _padRX, _padTX, NO_RTS_PIN, NO_CTS_PIN)
{
}

EUart::EUart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS) :
  is_waiting_for_read{ false }, sem_read{ 0 }, request_size{ 0 }, sem_write{ 0 }
#ifdef UNIT_TEST
  , readBusywaitCount { 0 }, writeBusywaitCount { 0 }
#endif
{
  sercom = _s;
  uc_pinRX = _pinRX;
  uc_pinTX = _pinTX;
  uc_padRX = _padRX;
  uc_padTX = _padTX;
  uc_pinRTS = _pinRTS;
  uc_pinCTS = _pinCTS;
}

void EUart::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void EUart::begin(unsigned long baudrate, uint16_t config)
{
  pinPeripheral(uc_pinRX, g_APinDescription[uc_pinRX].ulPinType);
  pinPeripheral(uc_pinTX, g_APinDescription[uc_pinTX].ulPinType);

  if (uc_padTX == UART_TX_RTS_CTS_PAD_0_2_3)
  {
    if (uc_pinCTS != NO_CTS_PIN)
    {
      pinPeripheral(uc_pinCTS, g_APinDescription[uc_pinCTS].ulPinType);
    }
  }

  if (uc_pinRTS != NO_RTS_PIN)
  {
    pinMode(uc_pinRTS, OUTPUT);

    EPortType rtsPort = g_APinDescription[uc_pinRTS].ulPort;
    pul_outsetRTS = &PORT->Group[rtsPort].OUTSET.reg;
    pul_outclrRTS = &PORT->Group[rtsPort].OUTCLR.reg;
    ul_pinMaskRTS = (1ul << g_APinDescription[uc_pinRTS].ulPin);

    *pul_outclrRTS = ul_pinMaskRTS;
  }

  sercom->initUART(UART_INT_CLOCK, SAMPLE_RATE_x16, baudrate);
  sercom->initFrame(extractCharSize(config), LSB_FIRST, extractParity(config), extractNbStopBit(config));
  sercom->initPads(uc_padTX, uc_padRX);

  sercom->enableUART();
}

void EUart::end()
{
  sercom->resetUART();
  rxBuffer.clear();
  txBuffer.clear();
}

void EUart::flush()
{
  while (txBuffer.available())
    ; // wait until TX buffer is empty

  sercom->flushUART();
}

void EUart::IrqHandler()
{
  if (sercom->isFrameErrorUART())
  {
    // frame error, next byte is invalid so read and discard it
    sercom->readDataUART();

    sercom->clearFrameErrorUART();
  }

  if (sercom->availableDataUART())
  {
    rxBuffer.store_char(sercom->readDataUART());

    if (uc_pinRTS != NO_RTS_PIN)
    {
      // RX buffer space is below the threshold, de-assert RTS
      if (rxBuffer.availableForStore() < RTS_RX_THRESHOLD)
      {
        *pul_outsetRTS = ul_pinMaskRTS;
      }
    }
    if (is_waiting_for_read)
    {
      is_waiting_for_read = false;
      sem_read.putFromISR();
    }
  }

  if (sercom->isDataRegisterEmptyUART())
  {
    if (txBuffer.available())
    {
      uint8_t data = txBuffer.read_char();

      sercom->writeDataUART(data);
      if (request_size > 0)
      {
        --request_size;
        if (request_size == 0)
        {
          sem_write.putFromISR();
        }
      }
    }
    else
    {
      sercom->disableDataRegisterEmptyInterruptUART();
    }
  }

  if (sercom->isUARTError())
  {
    sercom->acknowledgeUARTError();
    // TODO: if (sercom->isBufferOverflowErrorUART()) ....
    // TODO: if (sercom->isParityErrorUART()) ....
    sercom->clearStatusUART();
  }
}

int EUart::available()
{
  return rxBuffer.available();
}

int EUart::availableForWrite()
{
  return txBuffer.availableForStore();
}

int EUart::peek()
{
  return rxBuffer.peek();
}

int EUart::read()
{
  int c = rxBuffer.read_char();

  if (uc_pinRTS != NO_RTS_PIN)
  {
    // if there is enough space in the RX buffer, assert RTS
    if (rxBuffer.availableForStore() > RTS_RX_THRESHOLD)
    {
      *pul_outclrRTS = ul_pinMaskRTS;
    }
  }

  return c;
}

size_t EUart::write(const uint8_t data)
{
  if (sercom->isDataRegisterEmptyUART() && txBuffer.available() == 0)
  {
    sercom->writeDataUART(data);
  }
  else
  {
    // spin lock until a spot opens up in the buffer
    while (txBuffer.isFull())
    {
      uint8_t interruptsEnabled = ((__get_PRIMASK() & 0x1) == 0);

      if (interruptsEnabled)
      {
        uint32_t exceptionNumber = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);

        if (exceptionNumber == 0 ||
            NVIC_GetPriority((IRQn_Type)(exceptionNumber - 16)) > SERCOM_NVIC_PRIORITY)
        {
          // no exception or called from an ISR with lower priority,
          // wait for free buffer spot via IRQ
          continue;
        }
      }

      // interrupts are disabled or called from ISR with higher or equal priority than the SERCOM IRQ
      // manually call the EUart IRQ handler when the data register is empty
      if (sercom->isDataRegisterEmptyUART())
      {
        IrqHandler();
      }
    }

    txBuffer.store_char(data);

    sercom->enableDataRegisterEmptyInterruptUART();
  }

  return 1;
}

SercomNumberStopBit EUart::extractNbStopBit(uint16_t config)
{
  switch (config & HARDSER_STOP_BIT_MASK)
  {
  case HARDSER_STOP_BIT_1:
  default:
    return SERCOM_STOP_BIT_1;

  case HARDSER_STOP_BIT_2:
    return SERCOM_STOP_BITS_2;
  }
}

SercomUartCharSize EUart::extractCharSize(uint16_t config)
{
  switch (config & HARDSER_DATA_MASK)
  {
  case HARDSER_DATA_5:
    return UART_CHAR_SIZE_5_BITS;

  case HARDSER_DATA_6:
    return UART_CHAR_SIZE_6_BITS;

  case HARDSER_DATA_7:
    return UART_CHAR_SIZE_7_BITS;

  case HARDSER_DATA_8:
  default:
    return UART_CHAR_SIZE_8_BITS;
  }
}

SercomParityMode EUart::extractParity(uint16_t config)
{
  switch (config & HARDSER_PARITY_MASK)
  {
  case HARDSER_PARITY_NONE:
  default:
    return SERCOM_NO_PARITY;

  case HARDSER_PARITY_EVEN:
    return SERCOM_EVEN_PARITY;

  case HARDSER_PARITY_ODD:
    return SERCOM_ODD_PARITY;
  }
}

void EUart::waitForRead()
{
  noInterrupts();
  if (available() > 0)
  {
    interrupts();
    return;
  }
  is_waiting_for_read = true;
  interrupts();
  sem_read.get(Semaphore::kWaitForever);
}

void EUart::waitForWrite(size_t n)
{
  if (n == 0)
  {
    return;
  }
  noInterrupts();
  size_t avail = static_cast<size_t>(txBuffer.availableForStore());
  if (n <= avail)
  {
    interrupts();
    return;
  }
  request_size = n - avail;
  interrupts();
  sem_write.get(Semaphore::kWaitForever);
}

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

UartTransport::UartTransport(HardwareSerialEx *uartDrv, unsigned long baudrate)
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
    waitMessage();

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
    // txBuffer.availableForStore() returns 255 when buffer empty, 
    // so sendSize shoud be less r\than or equal 255
    const uint32_t sendSize = min(size - sentSize, static_cast<uint32_t>(255));
    m_uartDrv->waitForWrite(sendSize);
    sentSize += m_uartDrv->write(&data[sentSize], sendSize);
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

void UartTransport::waitMessage()
{
  m_uartDrv->waitForRead();
}
