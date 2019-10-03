/**
\file
\brief Driver to CO2 using the Intelligent Infrared CO2 Module MH-Z19.

\author Ib Havn
\version 1.0.0

\defgroup mh_z19_driver Driver for MH-Z19 CO2 sensor
\{
\brief Driver to CO2 using the Intelligent Infrared CO2 Module MH-Z19.

The simple user manual for MH-Z19 can be found here <a href="https://www.winsen-sensor.com/d/files/PDF/Infrared%20Gas%20Sensor/NDIR%20CO2%20SENSOR/MH-Z19%20CO2%20Ver1.0.pdf">Intelligent Infrared CO2 Module
(Model: MH-Z19) User's Manual (Version: 1.0)</a>

The implementation works with interrupt, meaning that there are no busy-waiting involved.

\note Dependent on Serial Driver.

See \ref mh_z19_driver_quick_start.

\defgroup mh_z19_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup mh_z19_driver_basic_function Basic MH-Z19 driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.

\defgroup mh_z19_driver_advanced_function Advanced MH-Z19 driver functions
\brief Advanced functions.
These functions will not normally be used, and be very careful to know what you are doing using these functions.

\defgroup mh_z19_driver_driver_return_codes MH-Z19 driver Return codes
\brief Return codes from MH-Z19 driver functions.
\}
*/

#ifndef MH_Z19_H_
#define MH_Z19_H_
#include <stdbool.h>
#include <stdint.h>
#include <serial.h>

/**
\ingroup mh_z19_driver_driver_return_codes
\brief MH-Z19 Driver return codes.

These are the codes that can be returned from calls to the driver.
For more documentation of these error codes see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>
*/
typedef enum
	{
		MHZ19_OK 	/**< Everything went well */
		,MHZ19_NO_MEASSURING_AVAILABLE /**< No results are received from the sensor */
		,MHZ19_NO_SERIAL /**< No serial driver is specified */
		,MHZ19_PPM_MUST_BE_GT_999 /**< Span callibration is only allowed from ppm >= 1000 */
	} mh_z19_return_code_t;

/* ======================================================================================================================= */
/**
\ingroup mh_z19_driver_creation
\brief Create the driver.

Creates and initialize the MH-Z19 Driver.

\param[in] com_port to be used for communication with the RN2483 module.
\param[in] mh_z19_call_back function pointer to call back function, or NULL if no call back function is used.

The call back function must have this signature:
\code
void function_name(uint16_t ppm)
\endcode
*/
void mh_z19_create(e_com_port_t com_port,void(*mh_z19_call_back )(uint16_t ppm));

/* ======================================================================================================================= */
/**
\ingroup mh_z19_driver_basic_function
\brief Perform a new CO2 meassuring.

Ask the module to take a new meassuring.

\note Must be performed before a call to mh_z19_get_co2_ppm.

\return Result of the call - see mh_z19_return_code_t.
*/
mh_z19_return_code_t mh_z19_take_meassuring(void);

/* ======================================================================================================================= */
/**
\ingroup mh_z19_driver_basic_function
\brief Get latest CO2 value.

\param[out] ppm pointer to the variable where the ppm value will be returned.

\return Result of the call - see mh_z19_return_code_t.
*/
mh_z19_return_code_t mh_z19_get_co2_ppm(uint16_t *ppm);

/* ======================================================================================================================= */
/**
\ingroup mh_z19_driver_advanced_function
\brief Enable/disable auto calibration of the sensor.

\note This must unly be used if the sensor is installed outdoor!!

\param[in] on true: Enable auto calibration, false: Disable auto calibration.

\return Result of the call - see mh_z19_return_code_t.
*/
mh_z19_return_code_t mh_z19_set_auto_calibration(bool on);

/* ======================================================================================================================= */
/**
\ingroup mh_z19_driver_advanced_function
\brief Zero calibration of the sensor.

\note Don't use if you don't know what you are doing - Can destroy the module!!!!

\return Result of the call - see mh_z19_return_code_t.
*/
mh_z19_return_code_t mh_z19_calibrate_zero_point(void);

/* ======================================================================================================================= */
/**
\ingroup mh_z19_driver_advanced_function
\brief Span calibration of the sensor.

\note Don't use if you don't know what you are doing - Can destroy the module!!!!

\param[in] ppm the CO2 ppm to calibrate the sensor to.

\return Result of the call - see mh_z19_return_code_t.
*/
mh_z19_return_code_t mh_z19_calibrate_span_point(uint16_t ppm);

/**
\page mh_z19_driver_quick_start Quick start guide for MH-Z19 CO2 Driver

This is the quick start guide for the \ref mh_z19_driver, with
step-by-step instructions on how to configure and use the driver in simple use cases.

The use cases contain several code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section mh_z19_use_cases MH-Z19 Driver use cases
- \ref mh_z19_initialise
- \ref lora_perform_co2_meassuring

\section mh_z19_initialise Initialise the driver
-# The following must be added to the project:
\code
#include <mh_z19.h>
\endcode

-# Create a call back function:
\code
void my_co2_call_back(uint16_t ppm)
{
	// Here you can use the CO2 ppm value
}
\endcode

 The call back function will be called by the driver when a new CO2 value is returned by the sensor.

-# Add to application initialization:
Initialise the driver:
\code
	// The first parameter is the USART port the MH-Z19 sensor is connected to - in this case USART3
	// The second parameter is the address of the call back function
	mh_z19_create(ser_USART3, my_co2_call_back); 
\endcode

\section lora_perform_co2_meassuring Perform a CO2 measuring

In this use case, a CO2 measuring will be performed.

\note The driver must be initialised \ref mh_z19_initialise before a measuring can be performed.

-# Define a variable to get the CO2 ppm in and a return code variable.
\code
	uint16_t ppm;
	mh_z19_return_code_t rc;
\endcode

-# Ask the driver to perform measuring.
\code
	rc = mh_z19_take_meassuring();
	if (rc != OK)
	{
		// Something went wrong
	}
 \endcode

-# When the driver has received the new ppm value from the sensor the specified call back function (see \ref mh_z19_initialise) will be called.
*/

#endif /* MH_Z19_H_ */