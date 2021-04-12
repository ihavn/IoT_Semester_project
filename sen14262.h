/**
\file
\brief Driver to a SEN-14262 sound sensor

\author Ib Havn
\version 1.0.0

\defgroup sen14262_driver Driver for SEN-14262 sound sensor
\{
\brief Driver to the SEN-14262 sound sensor from Sparkfun.

Description of the SEN-14262 can be found here <a href="https://learn.sparkfun.com/tutorials/sound-detector-hookup-guide?_ga=2.165119417.704281120.1553773860-2113010154.1549288517#introduction">Sound Detector Hookup Guide</a>

The implementation works with interrupt, meaning that there are no busy-waiting involved.

See \ref sen14262_driver_quick_start.

\defgroup sen14262_driver_creation Functions to initialize the driver.
\brief How to initialise the driver.

\defgroup sen14262_driver_basic_function Basic driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.
\}
*/

#ifndef SEN14262_H_
#define SEN14262_H_

#include <stdbool.h>
#include <stdint.h>
/* ======================================================================================================================= */
/**
\ingroup sen14262_driver_creation
\brief Initialise the sound sensor driver.

Initialise the driver.

This should only be called once during initialisation of the application.
*/
void sen14262_initialise(void);

/* ======================================================================================================================= */
/**
\ingroup sen14262_driver_basic_function
\brief Gives the latest measured envelope value from the sensor.

\return Latest measured envelope sound value.
*/
uint16_t sen14262_envelope(void);

/* ======================================================================================================================= */
/**
\ingroup sen14262_driver_basic_function
\brief State of the gate signal on the sensor.

\return State of gate signal.
*/
bool sen14262_gate(void);

/**
\page sen14262_driver_quick_start Quick start guide for SEN14262 Sound Sensor Driver

This is the quick start guide for the \ref sen14262_driver, with
step-by-step instructions on how to configure and use the driver in simple use cases.

The use cases contain several code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section sen14262_use_cases SEN14262 Driver use cases
- \ref sen14262_initialise
- \ref sen14262_perform_measurings

\section sen14262_initialise Initialise the driver
-# The following must be added to the project:
\code
#include <sen14262.h>
\endcode

-# Add to application initialization:
Initialise the driver:
\code
	sen14262_initialise(); 
\endcode

As soon as the driver is initialised it will automatically start measuring the envelope sound signal from the sensor.

\section sen14262_perform_measurings Perform a Sound measuring

In this use case, the latest envelope sound measuring will be retrieved from the driver.

\note The driver must be initialised \ref sen14262_initialise before a measuring can be performed.

-# Define a variable to store the sound value into.
\code
	uint16_t lastSoundValue;
\endcode

-# Get the latest measuring from the driver.
\code
	lastSoundValue = sen14262_envelope();
 \endcode
*/
#endif /* SEN14262_H_ */