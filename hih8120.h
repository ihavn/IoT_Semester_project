/**
\file

\author Ib Havn
\version 1.0.0

\defgroup hih8120_driver HIH8120 Driver
\{
\brief Honeywell HumidIcon Digital Humidity/Temperature Sensor HIH8220 driver.

The user manual for HIH8120 can be found here <a href="https://sensing.honeywell.com/i2c-comms-humidicon-tn-009061-2-en-final-07jun12.pdf">I2C Communication with the Honeywell HumidIcon Digital Humidity/Temperature Sensors (Version: 1.0)</a>

The implementation works with interrupt, meaning that there are no busy-waiting involved.

See \ref hih8120_driver_quickstart.

\defgroup hih8120_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup hih8120_driver_function HIH8120 driver functions
\brief Commonly used driver functions.
Here you you will find the functions you will need to work with the driver.

\defgroup hih8120_driver_return_codes HIH8120 driver Return codes
\brief Return codes from HIH8120 driver functions.
\}
*/

#ifndef HIH8120_H_
#define HIH8120_H_
#include <stdbool.h>

/**
\ingroup hih8120_driver_return_codes
\brief HIH8120 Driver return codes.

These are the codes that can be returned from calls to the driver.
*/
typedef enum hih8120DriverReturnCodes {
	HIH8120_OK	/**< Everything went well */
	,HIH8120_OUT_OF_HEAP /**< Abstract data type can't be instantiated */
	,HIH8120_DRIVER_NOT_CREATED /**< Driver must be created before use */
	,HIH8120_TWI_BUSY /**< The two wire/I2C interface is busy */
} hih8120DriverReturnCode_t;

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_creation
\brief Create the HIH8120 driver.

Creates and initialize the driver.

\note The driver must be destroyed when it is not needed anymore \see hih8120Destroy.

\return hih8120DriverReturnCode_t
\retval HIH8120_OK The driver is created.
\retval HIH8120_OUT_OF_HEAP There is not enough HEAP memory to create the driver.
*/
hih8120DriverReturnCode_t hih8120Create(void); 

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_creation
\brief Destroy the HIH8120 driver.

Destroys the driver after use. The HEAP memory used for the driver will be freed again.

\note The driver should only be destroyed when it is not needed anymore.

\return hih8120DriverReturnCode_t
\retval HIH8120_OK The driver is destroyed.
\retval HIH8120_DRIVER_NOT_CREATED The driver must be created before it can be destroyed.
*/
hih8120DriverReturnCode_t hih8120Destroy(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Test if result is ready.

\return boolean
\retval true last measuring is finished and the result is ready to be retrieved.
\retval false the measuring is in progress and the result is not ready yet.
*/
bool hih8120IsReady(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Wakes up the HIH8180 sensor.

\note After this call it is necessary to wait minimum 50 ms before a measuring can be done \see hih8120Meassure.

\return hih8120DriverReturnCode_t
\retval HIH8120_OK Wake up command send to the sensor.
\retval HIH8120_DRIVER_NOT_CREATED The driver must be created before use.
\retval HIH8120_TWI_BUSY The TWI/I2C bus is busy, try later.
*/
hih8120DriverReturnCode_t hih8120Wakeup(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Fetch the latest results from the HIH8180 sensor.

\note
After this call it is necessary to wait minimum 1 ms before the results can be retrieved with:
\see hih8120GetHumidityPercent_x10 
\see hih8120GetTemperature_x10 
\see hih8120GetHumidity 
\see hih8120GetTemperature.

\return hih8120DriverReturnCode_t
\retval HIH8120_OK Fetch command send to the sensor.
\retval HIH8120_DRIVER_NOT_CREATED The driver must be created before use.
\retval HIH8120_TWI_BUSY The TWI/I2C bus is busy, try later.
*/
hih8120DriverReturnCode_t hih8120Meassure(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Get latest measured relative humidity percent in tenths of %.

\return Relative humidity % [x10]
*/
uint16_t hih8120GetHumidityPercent_x10(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Get latest measured temperature in tenths of C.

\return Temperature C [x10]
*/
int16_t hih8120GetTemperature_x10(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Get latest measured relative humidity percent.

\return Relative humidity %
*/
float hih8120GetHumidity(void);

/* ======================================================================================================================= */
/**
\ingroup hih8120_driver_function
\brief Get latest measured temperature in C.

\return Temperature C
*/
float hih8120GetTemperature(void);

/**
\page hih8120_driver_quickstart Quick start guide for HIH8120 Driver

This is the quick start guide for the \ref hih8120_driver, with
step-by-step instructions on how to configure and use the driver in a
selection of use cases.

The use cases contain several code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section hih8120_driver_use_cases HIH8120 Driver use cases
- \ref hih8120_setup_use_case
- \ref hih8120_make_measuring

\section hih8120_setup_use_case Initialise the driver
The following must be added to the project:
- \code
#include <hih8120.h>
\endcode

Add to application initialization:
- Initialise the driver:
\code
if ( HIH8120_OK == hih8120Create() )
{
	// Driver created OK
	// Always check what hih8120Create() returns
}
\endcode

\section hih8120_make_measuring How to perform a new measuring with the sensor 

In this use case, the steps to perform a measuring is shown.

\note The driver must be created \ref hih8120_setup_use_case before a measuring is possible.

In this example these two variables will be used to store the results in

\code
	float humidity = 0.0;
	float temperature = 0.0;
\endcode

\subsection hih8120_make_measuring2 Perform the measuring
The following must be added to the application code:
-# Wake up the sensor from power down:
\code
	if ( HIH8120_OK != hih8120Wakeup() )
	{
		// Something went wrong
		// Investigate the return code further
	}
\endcode

 \note After the hih8120Wakeup() call the sensor will need minimum 50 ms to be ready! 

-# Poll the sensor for the results:
\code 
	if ( HIH8120_OK !=  hih8120Meassure() )
	{
		// Something went wrong
		// Investigate the return code further
	}
\endcode

 \note After the hih8120Meassure() call the two wire inteface (TWI) will need minimum 1 ms to fetch the results from the sensor!  

-# Get the results:
The result can now be retrieved by calling one of these methods hih8120GetHumidityPercent_x10(), hih8120GetTemperature_x10(), hih8120GetHumidity() or hih8120GetTemperature().

A small example 
\code 
	humidity = hih8120GetHumidity();
	temperature = hih8120GetTemperature();
\endcode
*/
#endif /* HIH8120_H_ */