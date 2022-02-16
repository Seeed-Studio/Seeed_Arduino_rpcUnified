/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_
#define _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_

#include "erpc_framed_transport.h"
#include "Arduino.h"
#include <stdlib.h>


/*!
 * @addtogroup uart_transport
 * @{
 * @file
 */

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpc {


#include "HardwareSerial.h"
#include "SERCOM.h"
#include "RingBuffer.h"
#include "erpc_threading.h"

#include <cstddef>

class HardwareSerialEx : public HardwareSerial
{
  public:
    virtual void begin(unsigned long) {}
    virtual void begin(unsigned long, uint16_t) {}
    virtual void end() {}
    virtual int available(void) = 0;
    virtual int peek(void) = 0;
    virtual int read(void) = 0;
    virtual void flush(void) = 0;
    virtual size_t write(uint8_t) = 0;
    using Print::write; // pull in write(str) and write(buf, size) from Print
    virtual operator bool() = 0;

    virtual void waitForRead() = 0;
    virtual void waitForWrite(size_t) = 0;
};

class EUart : public HardwareSerialEx
{
  public:
    EUart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX);
    EUart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS);
    void begin(unsigned long baudRate);
    void begin(unsigned long baudrate, uint16_t config);
    void end();
    int available();
    int availableForWrite();
    int peek();
    int read();
    void flush();
    size_t write(const uint8_t data);
    using Print::write; // pull in write(str) and write(buf, size) from Print

    void IrqHandler();

    operator bool() { return true; }

    virtual void waitForRead() override;
    virtual void waitForWrite(size_t n) override;

  private:
    SERCOM *sercom;
    RingBufferN<4096> rxBuffer;
    RingBuffer txBuffer;

    uint8_t uc_pinRX;
    uint8_t uc_pinTX;
    SercomRXPad uc_padRX;
    SercomUartTXPad uc_padTX;
    uint8_t uc_pinRTS;
    volatile uint32_t* pul_outsetRTS;
    volatile uint32_t* pul_outclrRTS;
    uint32_t ul_pinMaskRTS;
    uint8_t uc_pinCTS;

    volatile bool is_waiting_for_read;
    Semaphore sem_read;
    volatile size_t request_size;
    Semaphore sem_write;

    SercomNumberStopBit extractNbStopBit(uint16_t config);
    SercomUartCharSize extractCharSize(uint16_t config);
    SercomParityMode extractParity(uint16_t config);
};

/*!
 * @brief Very basic transport to send/receive messages via EUart.
 *
 * @ingroup uart_transport
 */
class UartTransport : public FramedTransport
{
public:
    /*!
     * @brief Constructor.
     *
     * @param[in] uartDrv Cmsis EUart.
     */
    UartTransport(HardwareSerialEx *uartDrv, unsigned long baudrate);

    /*!
     * @brief Destructor.
     */
    virtual ~UartTransport(void);

    /*!
     * @brief Initialize CMSIS EUart peripheral configuration structure with values specified in UartTransport
     * constructor.
     *
     * @retval kErpcStatus_InitFailed When EUart init function failed.
     * @retval kErpcStatus_Success When EUart init function was executed successfully.
     */
    virtual erpc_status_t init(void);

    virtual bool hasMessage(void);

    virtual void waitMessage(void) override;

protected:
    HardwareSerialEx *m_uartDrv; /*!< Access structure of the USART Driver */
    unsigned long m_baudrate;  /*!< EUart baud rate*/

private:
    /*!
     * @brief Receive data from EUart peripheral.
     *
     * @param[inout] data Preallocated buffer for receiving data.
     * @param[in] size Size of data to read.
     *
     * @retval kErpcStatus_ReceiveFailed EUart failed to receive data.
     * @retval kErpcStatus_Success Successfully received all data.
     */
    virtual erpc_status_t underlyingReceive(uint8_t *data, uint32_t size);

    /*!
     * @brief Write data to EUart peripheral.
     *
     * @param[in] data Buffer to send.
     * @param[in] size Size of data to send.
     *
     * @retval kErpcStatus_Success Always returns success status.
     */
    virtual erpc_status_t underlyingSend(const uint8_t *data, uint32_t size);
};

} // namespace erpc

/*! @} */

#endif // _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_
