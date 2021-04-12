/**
\file
\brief Driver to PIR Motion Detector Module

\author Ib Havn
\version 1.0.0

\defgroup sr501_driver Driver for PIR sensor
\{
\brief Driver to PIR sensor using the HC-SR501 PIR Motion Detector Module.

The datasheet for the HC-SR501 PIR Motion Detector can be found here <a href="https://www.mpja.com/download/31227sc.pdf">HC-SR501 PIR Motion Detector Product Description</a>

See \ref sr501_driver_quick_start.

\defgroup sr501_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup sr501_driver_basic_function Basic driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.
\}
*/

#ifndef HCSR501_H_
#define HCSR501_H_
#include <stdbool.h>

// Abstract Data Type (ADT)
typedef struct hcsr501_struct *hcsr501_p;

/* ======================================================================================================================= */
/**
\ingroup sr501_driver_creation
\brief Create the HC-SR501 driver.

Creates and initialize the driver.

\note The driver must be destroyed when it is not needed anymore \ref hcsr501_destroy.

\param[in] *port pointer to the PORT the PIR sensor is connected to (e.g. &PORTA).
\param[in] portPin in the PORT the PIR sensor is connected to (e.g. PA3).

\return instance pointer
*/
hcsr501_p hcsr501_create(volatile uint8_t *port, uint8_t portPin);

/* ======================================================================================================================= */
/**
\ingroup sr501_driver_creation
\brief Destroy the HC-SR501 driver.

Destroys the driver after use. The HEAP memory used for the driver will be freed again.

\note The driver should only be destroyed when it is not needed anymore.

\param[in] instance driver instance to be destroyed \ref hcsr501_create.
*/
void hcsr501_destroy(hcsr501_p instance);

/* ======================================================================================================================= */
/**
\ingroup sr501_driver_basic_function
\brief Tell if the sensor is detecting something.

\param[in] instance driver instance \ref hcsr501_create.

\return detecting status
\retval true Sensor detects something.
\retval false nothing is detected.
*/
bool hcsr501_isDetecting(hcsr501_p instance);

/**
\page sr501_driver_quick_start Quick start guide for HC-SR501 PIR-sensor Driver

This is the quick start guide for the \ref sr501_driver, with
step-by-step instructions on how to configure and use the driver in a
selection of use cases.

The use cases contain several code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section sr501_driver_use_cases HC-SR501 Driver use cases
- \ref sr501_setup_use_case
- \ref sr501_detect

\section sr501_setup_use_case Create and initialise the driver
The following must be added to the project:
- \code
#include <hcsr501.h>

hcsr501_p hcsr501Inst = NULL;
\endcode

Add to application initialization:
- Initialise the driver:
\code
hcsr501Inst = hcsr501_create(&PORTE, PE5);
if ( NULL != hcsr501Inst )
{
	// Driver created OK
	// If NULL is returned the driver is not created!!!
}
\endcode

\section sr501_detect How to test if the sensor detects anything

In this use case, the steps to perform a detection is shown.

\note The driver must be created \ref sr501_setup_use_case before a detection is possible.

\code
	if ( hcsr501_isDetecting(hcsr501Inst) )
	{
		// Something is detected
	}
	else 
	{
		// Nothing is detected
	}
\endcode

*/

#endif /* HCSR501_H_ */