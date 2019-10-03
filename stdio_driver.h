/**
\file

\author Ib Havn
\version 1.0.0

\defgroup stdio_driver Driver for connecting standard IO.
\{
\brief Driver to connect a USART to standard IO.

\defgroup stdio_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup stdio_driver_functions Stdio driver common functions.
\brief Normally used functions.
\}
*/
#ifndef STDIO_DRIVER_H_ 
#define STDIO_DRIVER_H_

#include <stdbool.h>

/* ======================================================================================================================= */
/**
\ingroup stdio_driver_creation
\brief Creates the stdio driver.

Creates and initialize the driver.

Connects stdin and stdout to the usart_no given. 

The USART are setup like this:  57600 baud,
8-bit Data bits, 1 Stop bit and No parity.

After this function is called, it is possible to use printf(), scanf etc.

\note This function must be called before using printf(), scanf etc.
\note Remember to enable global interrupt by calling sei() after the driver is created.

\param[in] usart_no no of the USART to setup and connect to stdin and stdout [0..3].
*/
void stdioCreate(uint8_t usart_no);

/* ======================================================================================================================= */
/**
\ingroup stdio_driver_functions

Check if there is any inputs ready in the input queue.
That can prevent a program from blocking when waiting for stdio.

\return true if input is waiting.
*/
bool stdioInputWaiting(void);

#endif /* STDIO_DRIVER_H_ */