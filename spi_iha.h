/*
* spi_iha.h
*
* Created: 16-03-2013 11:30:09
*  Author: IHA
*/


#ifndef SPI_IHA_H_
#define SPI_IHA_H_

// ------------- Includes -------------------
#include <inttypes.h>

#include "../fifo/fifo.h"
// ------------- Defines --------------------

// Abstract Data Type (ADT)
typedef struct spi_struct * spi_p;

/**
\ingroup spi_mode
@{
	@brief Set SPI in Master mode*/
	#define SPI_MODE_MASTER _BV(MSTR)
	/** @brief Set SPI in Slave mode*/
	#define SPI_MODE_SLAVE NOT_IMPLEMENTED
	/**
@}

\ingroup spi_clock
@brief Division for F_CPU to give SPI-Clock
@{
	*/
	#define SPI_CLOCK_DIVIDER_2 4
	#define SPI_CLOCK_DIVIDER_4 0
	#define SPI_CLOCK_DIVIDER_8 5
	#define SPI_CLOCK_DIVIDER_16 1
	#define SPI_CLOCK_DIVIDER_32 6
	#define SPI_CLOCK_DIVIDER_64 2
	#define SPI_CLOCK_DIVIDER_128 3
	/**
@}

\ingroup spi_data_order
@{
	@brief LSB sent first*/
	#define SPI_DATA_ORDER_LSB _BV(DORD)
	/** @brief MSB sent first*/
	#define SPI_DATA_ORDER_MSB 0
	/**
@}

@ingroup spi_return_codes
@{
	@brief The function succeeded. */
	#define SPI_OK 0
	/** @brief Not enough room in tx buffer to store value, or buffer is not used. */
	#define SPI_NO_ROOM_IN_TX_BUFFER 1
	/** @brief SPI bus is busy - nothing done. */
	#define SPI_BUSY 2
	/** @brief The specified instance is > SPI_MAX_NO_OF_INSTANCES or is not instantiated yet. */
	#define SPI_ILLEGAL_INSTANCE 3
	/**
@}
*/

// ------------- Prototypes -----------------
spi_p spi_new_instance(uint8_t mode, int8_t clock_divider, uint8_t spi_mode, uint8_t data_order, volatile uint8_t *cs_port, uint8_t cs_pin, uint8_t cs_active_level,
uint8_t rx_fifo_size, uint8_t tx_fifo_size, void(*handler_call_back )(spi_p, uint8_t));
uint8_t spi_send_byte(spi_p spi, uint8_t byte);
uint8_t spi_send_string(spi_p spi, uint8_t buf[], uint8_t len);
#endif /* SPI_IHA_H_ */