/*! @file serial.h

@author iha

@defgroup  serial_driver Driver for ATMEGA256x and ATMEGA324PA USARTs.
@{
@brief A driver for using the USARTs for serial communication.

@note Only implemented for 8,N,1 Data format!!!!

@}
*/
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

// Abstract Data Type (ADT)
typedef struct serial_struct *serial_p;

typedef struct {
	uint8_t * data;
	uint8_t len;
} serial_data_t;

// Serial return codes
typedef enum {
	SERIAL_OK,
	SERIAL_ILLEGAL_INSTANCE,
	SERIAL_NO_ROOM_IN_TX_BUFFER,
	SERIAL_RX_BUFFER_EMPTY
} serial_returnCode_t;

typedef enum
{
	ser_USART0 = 0,
	ser_USART1,
	ser_USART2,
	ser_USART3
} serial_comPort_t;

typedef enum
{
	ser_NO_PARITY,
	ser_ODD_PARITY,
	ser_EVEN_PARITY,
	ser_MARK_PARITY,
	ser_SPACE_PARITY
} serial_parity_t;

typedef enum
{
	ser_STOP_1,
	ser_STOP_2
} serial_stopBit_t;

typedef enum
{
	ser_BITS_5,
	ser_BITS_6,
	ser_BITS_7,
	ser_BITS_8
} serial_dataBit_t;

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_p serial_create(serial_comPort_t comPort, uint32_t baud, serial_dataBit_t dataBit, serial_stopBit_t stopBit, serial_parity_t parity, uint8_t rxFifoSize, uint8_t txFifoSize, void(*callBack )(serial_p, uint8_t));
/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_returnCode_t serial_sendBytes(serial_p handle, const serial_data_t data);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_returnCode_t serial_sendByte(serial_p handle, uint8_t byte);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_returnCode_t serial_getByte(serial_p handle, uint8_t *byte);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_returnCode_t serial_flushRx_Fifo(serial_p handle);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_returnCode_t serial_flushTxFifo(serial_p handle);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_returnCode_t serial_emptyRxFifo(serial_p handle, bool *isEmpty);
#endif

