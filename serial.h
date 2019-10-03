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
} e_serial_return_code_t;

typedef enum
{
	ser_USART0 = 0,
	ser_USART1,
	ser_USART2,
	ser_USART3
} e_com_port_t;

typedef enum
{
	ser_NO_PARITY,
	ser_ODD_PARITY,
	ser_EVEN_PARITY,
	ser_MARK_PARITY,
	ser_SPACE_PARITY
} e_parity_t;

typedef enum
{
	ser_STOP_1,
	ser_STOP_2
} e_stop_bit_t;

typedef enum
{
	ser_BITS_5,
	ser_BITS_6,
	ser_BITS_7,
	ser_BITS_8
} e_data_bit_t;

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
serial_p serial_new_instance(e_com_port_t com_port, uint32_t baud, e_data_bit_t data_bit, e_stop_bit_t stop_bit, e_parity_t parity, uint8_t rx_fifo_size, uint8_t tx_fifo_size, void(*handler_call_back )(serial_p, uint8_t));
/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
e_serial_return_code_t serial_send_bytes(serial_p handle, const serial_data_t data);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
e_serial_return_code_t serial_send_byte(serial_p handle, uint8_t byte);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
e_serial_return_code_t serial_get_byte(serial_p handle, uint8_t *byte);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
void serial_flush_rx_fifo(serial_p handle);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
void serial_flush_tx_fifo(serial_p handle);

/* ======================================================================================================================= */
/**
@ingroup serial_driver

@todo Documentation

*/
bool serial_rx_fifo_empty(serial_p handle);
#endif

