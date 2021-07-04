/*!
 * @addtogroup uart_transport
 * @{
 * @file
 */

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpc {

#include <cstddef>

class EUart : public Stream {
  protected:
    // Has any byte been written to the UART since begin()
    bool _written;

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.
    unsigned char _rx_buffer[4096];
    unsigned char _tx_buffer[256];

    serial_t _serial;

  public:
    EUart(void *peripheral, uint32_t _rx, uint32_t _tx);
    EUart(void *peripheral, PinName _rx, PinName _tx);
    void begin(unsigned long baud)
    {
      begin(baud, SERIAL_8N1);
    }
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
      return write((uint8_t)n);
    }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool()
    {
      return true;
    }

    // Interrupt handlers
    static void _rx_complete_irq(serial_t *obj);
    static int _tx_complete_irq(serial_t *obj);
  private:
    bool _rx_enabled;
    uint8_t _config;
    unsigned long _baud;
    void init(PinName _rx, PinName _tx);
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
    UartTransport(EUart *uartDrv, unsigned long baudrate);

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

protected:
    EUart *m_uartDrv; /*!< Access structure of the USART Driver */
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