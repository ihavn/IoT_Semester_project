/**
\file
\brief Driver for connecting standard IO to a serial port

\author Ib Havn
\version 1.0.0

\defgroup stdio_driver Driver for connecting standard IO.
\{
\brief Driver to connect a USART to standard IO.

\defgroup stdio_driver_creation Functions to create and initialize the driver.
\brief How to initialise the driver.

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
\brief Initialise the stdio driver.

Initialise the driver.

Connects stdin and stdout to the usartNo given. 

The USART are setup like this:  57600 baud,
8-bit Data bits, 1 Stop bit and No parity.

After this function is called, it is possible to use printf(), scanf etc.

\note This function must be called before using printf(), scanf etc.
\note Remember to enable global interrupt by calling sei() after the driver is initialised.

\param[in] usartNo no of the USART to setup and connect to stdin and stdout [0..3].
*/
void stdio_initialise(uint8_t usartNo);

/* ======================================================================================================================= */
/**
\ingroup stdio_driver_functions

Check if there is any inputs ready in the input queue.
That can prevent a program from blocking when waiting for stdio.

\return true if input is waiting.
*/
bool stdio_inputIsWaiting(void);

#endif /* STDIO_DRIVER_H_ */