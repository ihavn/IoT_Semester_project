/** \file
\brief Driver for SPI communication.
\author Ib Havn
\version 2.0.0

\defgroup spi SPI Driver
\{
The implementation works with interrupt, meaning that there are no busy-waiting involved.

\note Only implemented for Master mode.

\defgroup spi_config SPI Configuration
\brief Used to configure the SPI driver.

\defgroup spi_function SPI Functions
\brief Commonly used SPI functions.
Here you you will find the functions you will need.

\defgroup spi_return_codes SPI Return codes
\brief Return values from SPI functions.
\}
*/

#ifndef SPI_IHA_H_
#define SPI_IHA_H_

// ------------- Includes -------------------
#include <inttypes.h>

#include <fifo.h>
// ------------- Defines --------------------

// Abstract Data Type (ADT)
typedef struct spi_struct * spi_p;

/**
\ingroup spi_config
\brief SPI Master/Slave modes.
*/
typedef enum {
	SPI_MODE_MASTER = _BV(MSTR) /**< Set SPI in Master mode */
	,SPI_MODE_SLAVE /**< Set SPI in Slave mode - NOT_IMPLEMENTED*/ 
} spi_masterSlaveMode_t;

/**
\ingroup spi_config
\brief SPI Clock.

Division for the F_CPU to give SPI-Clock
*/
typedef enum {
	SPI_CLOCK_DIVIDER_2 = 4
	,SPI_CLOCK_DIVIDER_4 = 0
	,SPI_CLOCK_DIVIDER_8 = 5
	,SPI_CLOCK_DIVIDER_16 = 1
	,SPI_CLOCK_DIVIDER_32 = 6
	,SPI_CLOCK_DIVIDER_64 = 2
	,SPI_CLOCK_DIVIDER_128 = 3
} spi_clockDivider_t;

/**
\ingroup spi_config
\brief SPI modes.
*/
typedef enum {
	SPI_MODE_0 /**< Set SPI in MODE 0 */
	,SPI_MODE_1 /**< Set SPI in MODE 1 */
	,SPI_MODE_2 /**< Set SPI in MODE 2 */
	,SPI_MODE_3 /**< Set SPI in MODE 3 */
} spi_mode_t;
/**
\ingroup spi_config
\brief Order to sent bits in.
*/
typedef enum{
	SPI_DATA_ORDER_LSB = _BV(DORD) /**< LSB sent first */
	,SPI_DATA_ORDER_MSB = 0 /**< MSB sent first */
} spi_dataOrder_t;

/**
\ingroup spi_driver_return_codes
\brief SPI Driver return codes.

These are the codes that can be returned from calls to the driver.
*/
typedef enum {
	SPI_OK = 0 /**< Everything OK */
	,SPI_NO_ROOM_IN_TX_BUFFER = 1 /**< Not enough room in TX buffer to store value, or buffer is not defined. */
	,SPI_BUSY = 2 /**< SPI bus is busy - nothing done. */
	,SPI_ILLEGAL_INSTANCE = 3 	/**< The specified instance is > SPI_MAX_NO_OF_INSTANCES or is not instantiated yet. */
} spi_returnCode_t;


/* ======================================================================================================================= */
/**
\ingroup spi_function
\brief Setup and initialize new SPI instance.

Setup and create SPI instance according to parameters.

\note This must be called exactly once per new SPI instance.
\note Slave mode is not implemented!!!

\return handle to the SPI instance created. Should be used as parameter to the send() functions.

\param masterSlaveMode SPI in Master or Slave mode see \ref spi_masterSlaveMode_t
\param clockDivider defines the SPI clock divider see \ref spi_clockDivider_t
\param spiMode defines the SPI mode to use [0..3] \see spi_mode_t \n
|spi_mode | CPOL | CPHA |Clock Behavior|
|:---:    |:----:|:----:|:----------|
| 0       | 0    | 0    |Low when idle, samples on leading edge|
| 1       | 0    | 1    |Low when idle, samples on trailing edge|
| 2       | 1    | 0    |High when idle, samples on leading edge|
| 3       | 1    | 1    |High when idle, samples on trailing edge|

\param dataOrder Send LSB or MSB first \see spi_dataOrder_t
\param *csPort port register for the CS pin - Ex. &PORTB. If no CS is used then NULL
\param csPin that is connected to the slaves CS - Ex. PB6. If no CS is used then don't care
\param  csActiveLevel 
	- 0: CS active when low.
	- 1: CS active when high.
\param rxFifoSize 0: No receive buffer used, means no data will be received from slave or the size.
\param txFifoSize 0: No transmit buffer used, means that only one byte can be transmitted at the time or the size.
\param *callBack pointer to a call back function that will be called when the driver receives a byte from the SPI bus. The function should have the following signature:\n
\code
void handlerName(spi_p spiInstance, uint8_t rxByte)
\endcode
	- 0: no call back function will be called.
*/
spi_p spi_create(spi_masterSlaveMode_t masterSlaveMode, spi_clockDivider_t clockDivider, spi_mode_t spiMode, spi_dataOrder_t dataOrder, volatile uint8_t *csPort, uint8_t csPin, uint8_t csActiveLevel,
uint8_t rxFifoSize, uint8_t txFifoSize, void(*callBack )(spi_p, uint8_t));

/**
\ingroup spi_function
\brief Send a single byte to the SPI bus.

\param spi SPI instance to send to.
\param byte to be send.

\return SPI function return code see \ref spi_returnCode_t
*/
spi_returnCode_t spi_sendByte(spi_p spi, uint8_t byte);

/**
\ingroup spi_function
\brief Send an array of bytes to to the SPI bus.

\note Can only be used if tx_fifo_size > 0 \ref spi_create

\param spi SPI instance to send to.
\param *buf pointer to buffer to be send.
\param len no of bytes to send.

\return SPI function return code see \ref spi_returnCode_t
*/
spi_returnCode_t spi_sendBytes(spi_p spi, uint8_t buf[], uint8_t len);
#endif /* SPI_IHA_H_ */