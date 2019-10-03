/**
\file

\author Ib Havn
\version 1.0.0

\defgroup display_7seg_driver 7 Segment Display Driver
\{
\brief Driver for the 4 digit 7-segment display on the VIA MEGA2560 Shield.

The implementation works with interrupt, meaning that there are no busy-waiting involved.

See \ref display_7seg_driver_quickstart.

\defgroup display_7seg_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup display_7seg_driver_function 7-segment Display driver functions
\brief Commonly used driver functions.
Here you you will find the functions you will need to work with the driver.
\}
*/
#ifndef _DISPLAY_7SEG_H_
#define _DISPLAY_7SEG_H_

/* ======================================================================================================================= */
/**
\ingroup display_7seg_driver_creation
\brief Create the 7-segment Display driver.

Creates and initialize the driver.

\note When the driver is created the display will be in power-down mode \see display_7seg_power_up and \see display_7seg_power_down.

\param[in] display_done_call_back function pointer to call back function, or NULL if no call back function is used.

The Call back function will be called each time the display driver releases the SPI bus. This allows other drivers to use the driver.

The call back function must have this signature:
\code
void function_name(void)
\endcode
*/
void display_7seg_init(void (*display_done_call_back)(void));

/* ======================================================================================================================= */
/**
\ingroup display_7seg_driver_function
\brief Display value on the 7-segment Display.


\note When the driver is created the display will be in power-down mode \see display_7seg_power_up and \see display_7seg_power_down.

\param[in] value the value to shown on the display.
\param[in] no_of_decimals The number of digits after the decimal point.
*/
void display_7seg_display(float value, uint8_t no_of_decimals);

/* ======================================================================================================================= */
/**
\ingroup display_7seg_driver_function
\brief Power up the 7-segment display.

\note When the driver is created the display will be in power-down mode \see display_7seg_power_up and \see display_7seg_power_down.
*/
void display_7seg_power_up(void);

/* ======================================================================================================================= */
/**
\ingroup display_7seg_driver_function
\brief Power down the 7-segment display.

\note When the driver is created the display will be in power-down mode \see display_7seg_power_up and \see display_7seg_power_down.
*/
void display_7seg_power_down(void);

/**
\page display_7seg_driver_quickstart Quick start guide for 7-segment Display Driver

This is the quick start guide for the \ref display_7seg_driver, with
step-by-step instructions on how to configure and use the driver in a simple use case.

The use cases contain code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section display_7seg_use_cases 7-segment Display Driver use cases
- \ref display_7seg_driver_initialise
- \ref display_7seg_driver_show_number

\section display_7seg_driver_initialise Initialise the driver
-# The following must be added to the project:
\code
#include <display_7seg.h>
\endcode

-# Add to application initialization:
Initialise the driver:
\code
	// Here the call back function is not needed
	display_7seg_init(NULL); 

	// Power up the display
	display_7seg_power_up();
\endcode

\section display_7seg_driver_show_number Show numbers on the display

\note The driver must be initialised \ref display_7seg_driver_initialise and powered up \ref display_7seg_power_up before the display can be used.
\note 

-# Show &Pi; with two decimals on the display.
\code
	display_7seg_display(3.14159265359, 2);
\endcode
*/

#endif /* _DISPLAY_7SEG_H_ */
