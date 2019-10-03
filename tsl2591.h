/**
\file

\author Ib Havn
\version 1.0.0

\defgroup tsl2591_driver Driver for TSL2591 Light sensor
\{
\brief Driver for the TSL2591 Light sensor.

A little about light. Light is normally measured in LUX.
Indoor and outdoor light conditions:
| LUX | Condition |
| :---- | :----- |
| 0.002 lux | Moonless clear night sky |
| 0.2 lux	| Design minimum for emergency lighting (AS2293) |
| 0.27 - 1 lux | Full moon on a clear night |
| 3.4 lux	| Dark limit of civil twilight under a clear sky |
| 50 lux | Family living room |
| 80 lux | Hallway/toilet |
| 100 lux	| Very dark overcast day |
| 300 - 500 | lux	Sunrise or sunset on a clear day. Well-lit office area |
| 1,000 lux |Overcast day; typical TV studio lighting |
| 10,000 - 25,000 lux	| Full daylight (not direct sun) |
| 32,000 - 130,000 lux | Direct sunlight |

The Data-sheet for TSL2591 can be found here <a href="https://ams.com/documents/20143/36005/TSL2591_DS000338_6-00.pdf/090eb50d-bb18-5b45-4938-9b3672f86b80">TSL2591 Datasheet - June 2018</a>

The implementation works with interrupt, meaning that there are no busy-waiting involved.
\note This driver are using a call-back function to inform about completion of many of the driver functions. 
It is described in the individual functions documentation if the function will call the call-back function on completion.

\note Dependent on TWI-Driver.

See \ref tsl2591_driver_quick_start.

\defgroup tsl2591_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup tsl2591_driver_basic_function Basic TSL2591 driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.
\note Interrupt must be enabled with sei() before any of the functions in this section must be called!!

\defgroup tsl2591_driver_structs Configuration structs for the TSL2591 driver functions
\brief Commonly used structs.

\defgroup tsl2591_driver_return_codes TSL2591 driver Return codes
\brief Return codes from TSL2591 driver functions.
\}
*/

#ifndef TSL2591_H_
#define TSL2591_H_

#include <stdbool.h>

/**
\ingroup tsl2591_driver_structs
\brief TSL2591 Gain configuration.

These are the values that must be used to change the gain of the sensor (\ref tsl2591SetGainAndIntegrationTime)
*/
typedef enum {
	TSL2591_GAIN_LOW = 0x00 /**< Low gain (x1) - use in bright light (default) */
	,TSL2591_GAIN_MED = 0x10 /**< Medium gain (x25) - use in medium light */
	,TSL2591_GAIN_HIGH = 0x20 /**< High gain (x428) - use in very dimmed light */
	,TSL2591_GAIN_MAX = 0x30 /**< Maximum gain (x9876) - use in darkness */
} tsl2591Gain_t;

/**
\ingroup tsl2591_driver_structs
\brief TSL2591 Integration time configuration.

These are the values that must be used to change the integration time of the sensor (\ref tsl2591SetGainAndIntegrationTime)
The values are self explaining.
*/
typedef enum {
	TSL2591_INTEGRATION_TIME_100MS = 0x00 /**< Use in bright light (default) */
	,TSL2591_INTEGRATION_TIME_200MS = 0x01 /**< Use in medium light */
	,TSL2591_INTEGRATION_TIME_300MS = 0x02 /**< Use in medium light */
	,TSL2591_INTEGRATION_TIME_400MS = 0x03 /**< Use in dim light */
	,TSL2591_INTEGRATION_TIME_500MS = 0x04 /**< Use in dim light */
	,TSL2591_INTEGRATION_TIME_600MS = 0x05 /**< Use in darkness */
} tsl2591IntegrationTime_t;

/**
\ingroup tsl2591_driver_structs
\brief TSL2591 Light measure data struct.

The combined light result (\ref tsl2591GetCombinedDataRaw)

\note These data are the raw values coming directly from the sensor.
*/
typedef struct 
{
	uint16_t fullSpectrumRaw; /**< Full spectrum (Visible and Infrared) */
	uint16_t infraredRaw; /**< Infrared spectrum */
	uint16_t visibleRaw; /**< Visible spectrum */
} tsl2591CombinedData_t;

/**
\ingroup tsl2591_driver_return_codes
\brief TSL2591 Driver return codes.

These are the codes that can be returned from calls to the driver.
*/
typedef enum 
{
	TSL2591_OK /**< Everything went well */
	,TSL2591_DATA_READY /**< Data is ready from the last call to \ref tsl2591FetchData */
	,TSL2591_DEV_ID_READY  /**< Device ID is ready from the last call to \ref tsl2591FetchDeviceId */
	,TSL2591_OVERFLOW  /**< The last measuring is in overflow - consider a lower gain */
	,TSL2591_UNDERFLOW  /**< The last measuring is in underflow - consider a higher gain */
	,TSL2591_BUSY /**< The driver is busy or the TWI-driver is busy */
	,TSL2591_ERROR /**< A non specified error occurred */
	,TSL2591_DRIVER_NOT_CREATED /**< The driver is used before it is created \ref tsl2591Create */
	,TSL2591_OUT_OF_HEAP /**< There is not enough HEAP memory to create the driver */
} tsl2591ReturnCode_t;

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_creation
\brief Create the driver.

Creates and initialize the TSL2591 Driver.

\note This function will only create the driver! Be sure to enabled/powered up (\ref tsl2591Enable) the sensor before it can perform measuring's.

\param[in] callBack function pointer to call back function, or NULL if no call back function is used.

The call back function must have this signature:
\code
void function_name(tsl2591ReturnCode_t rc)
\endcode

The callback function will be called every time a command involving communication with the sensor is completed.
*/
tsl2591ReturnCode_t tsl2591Create(void(*callBack)(tsl2591ReturnCode_t));

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_creation
\brief Destroy the TSL2591 driver.

Destroys the driver after use. The HEAP memory used for the driver will be freed again.

\note The driver should only be destroyed when it is not needed anymore.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The driver is destroyed.
\retval TSL2591_DRIVER_NOT_CREATED The driver must be created before it can be destroyed.
*/
tsl2591ReturnCode_t tsl2591Destroy(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Enable/Power up the TSL2591 sensor.

\note The sensor is not ready before the driver calls the call-back function with TSL2591_OK as argument.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The enable sensor/powered up command is send to the sensor.
\retval TSL2591_BUSY The driver is busy.
\retval TSL2591_DRIVER_NOT_CREATED The driver is not created - and can not be used!!
*/
tsl2591ReturnCode_t tsl2591Enable(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Disable/Power down the TSL2591 sensor.

\note The sensor is not powered down before the driver calls the call-back function with TSL2591_OK as argument.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The disable sensor/powered down command is send to the sensor.
\retval TSL2591_BUSY The driver is busy.
\retval TSL2591_DRIVER_NOT_CREATED The driver is not created - and can not be used!!
*/
tsl2591ReturnCode_t tsl2591Disable(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Start fetching the TSL2591 sensor's device ID.

\note The ID is not available before the driver calls the call-back function with TSL2591_DEV_ID_READY as argument.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The fetch command is send to the sensor. Await the callback before the ID is retrieved with \ref tsl2591GetDeviceId.
\retval TSL2591_BUSY The driver is busy.
\retval TSL2591_DRIVER_NOT_CREATED The driver is not created - and can not be used!!
*/
tsl2591ReturnCode_t tsl2591FetchDeviceId(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the TSL2591 sensor's device ID.

\note The sensors ID will always be 0x50.

\return The sensor's latest fetched device ID.
*/
uint8_t tsl2591GetDeviceId(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Sets the TSL2591 sensor's Gain and Integration time.

The sensor supports four gain and six different integration times settings.

| Gain |||
| :---- | :----- | :----- |
| <b>Gain</b> | <b>Value</b> | <b>Use When</b> |
| x1 | TSL2591_GAIN_LOW | Bright light (default) |
| x25 | TSL2591_GAIN_MED | Medium light |
| x428 | TSL2591_GAIN_HIGH | Very dimmed light |
| x9876 | TSL2591_GAIN_MAX | Darkness |


| Integration Times (measuring time) |||
| :---- | :----- | :----- |
| <b>Integration time</b> | <b>Value</b> | <b>Use When</b> |
| 100 ms | TSL2591_INTEGRATION_TIME_100MS | Bright light (default) |
| 200 ms | TSL2591_INTEGRATION_TIME_200MS | Medium light |
| 300 ms | TSL2591_INTEGRATION_TIME_300MS | Medium light |
| 400 ms | TSL2591_INTEGRATION_TIME_400MS | Dim light |
| 500 ms | TSL2591_INTEGRATION_TIME_500MS | Dim light |
| 600 ms | TSL2591_INTEGRATION_TIME_600MS | Darkness |

The sensor's gain and integration time are set to TSL2591_GAIN_LOW (x1) and TSL2591_INTEGRATION_TIME_100MS on Power On Reset (POR).

\note The gain and integration time is not set before before the driver calls the call-back function with TSL2591_OK as argument.

\param[in] gain the wanted gain \ref tsl2591Gain_t.
\param[in] integrationTime the wanted integration time \ref tsl2591IntegrationTime_t.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The set gain and integration time command is send to the sensor.
\retval TSL2591_BUSY The driver is busy.
\retval TSL2591_DRIVER_NOT_CREATED The driver is not created - and can not be used!!
*/
tsl2591ReturnCode_t tsl2591SetGainAndIntegrationTime(tsl2591Gain_t gain, tsl2591IntegrationTime_t integrationTime);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the TSL2591 sensor's current gain setting.

\return The sensor's current gain.
*/
tsl2591Gain_t tsl2591GetGain(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the TSL2591 sensor's current integration time.

\return The sensor's current integration time.
*/
tsl2591IntegrationTime_t tsl2591GetIntegrationTime(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Start fetching the TSL2591 sensor's light data.

\note The sensors data is not ready before the driver calls the call-back function with TSL2591_DATA_READY as argument.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The fetch command is send to the sensor. Await the callback before the light data is retrieved with \ref tsl2591GetVisibleRaw, \ref tsl2591GetInfraredRaw, \ref tsl2591GetFullSpectrumRaw, \ref tsl2591GetCombinedDataRaw or \ref tsl2591GetLux.
\retval TSL2591_BUSY The driver is busy.
\retval TSL2591_DRIVER_NOT_CREATED The driver is not created - and can not be used!!
*/
tsl2591ReturnCode_t tsl2591FetchData(void);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the latest visible light spectrum as raw data fetched from the TSL2591 sensor.

\param[out] *visible pointer to the variable where the data will be stored.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The light data retrieved OK.
\retval TSL2591_OVERFLOW The last measuring is in overflow - consider a lower gain.
\retval TSL2591_OVERFLOW The last measuring is in underflow - consider a higher gain.
*/
tsl2591ReturnCode_t tsl2591GetVisibleRaw(uint16_t *visible);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the latest infrared light spectrum as raw data fetched from the TSL2591 sensor.

\param[out] *infrared pointer to the variable where the data will be stored.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The light data retrieved OK.
\retval TSL2591_OVERFLOW The last measuring is in overflow - consider a lower gain.
\retval TSL2591_OVERFLOW The last measuring is in underflow - consider a higher gain.
*/
tsl2591ReturnCode_t tsl2591GetInfraredRaw(uint16_t *infrared);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the latest full light spectrum as raw data fetched from the TSL2591 sensor.

\param[out] *fullSpectrum pointer to the variable where the data will be stored.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The light data retrieved OK.
\retval TSL2591_OVERFLOW The last measuring is in overflow - consider a lower gain.
\retval TSL2591_OVERFLOW The last measuring is in underflow - consider a higher gain.
*/
tsl2591ReturnCode_t tsl2591GetFullSpectrumRaw(uint16_t *fullSpectrum);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the latest combined light spectrum's as raw data fetched from the TSL2591 sensor.

\param[out] *combinedData pointer to the struct variable where the data will be stored \ref tsl2591CombinedData_t.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The light data retrieved OK.
\retval TSL2591_OVERFLOW The last measuring is in overflow - consider a lower gain.
\retval TSL2591_OVERFLOW The last measuring is in underflow - consider a higher gain.
*/
tsl2591ReturnCode_t tsl2591GetCombinedDataRaw(tsl2591CombinedData_t *combinedData);

/* ======================================================================================================================= */
/**
\ingroup tsl2591_driver_basic_function
\brief Retrieve the latest visible light spectrum as lux fetched from the TSL2591 sensor.

\note Fetch of the TSL2591 sensor's light data must be performed \ref tsl2591FetchData before this function can be used.

\param[out] *lux pointer to the variable where the data will be stored.

\return tsl2591ReturnCode_t
\retval TSL2591_OK The light data retrieved OK.
\retval TSL2591_OVERFLOW The last measuring is in overflow - consider a lower gain.
*/
tsl2591ReturnCode_t tsl2591GetLux(float *lux);

/**
\page tsl2591_driver_quick_start Quick start guide for TSL2591 Light sensor Driver

This is the quick start guide for the \ref tsl2591_driver, with
step-by-step instructions on how to configure and use the driver in a
selection of use cases.

The use cases contain several code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section tsl2591_driver_use_cases TSL2591 Driver use cases
- \ref tsl2591_setup_use_case
- \ref tsl2591_make_measuring

\section tsl2591_setup_use_case Initialise the driver
The following must be added to the project:
\code
#include <tsl2591.h>
\endcode

Call back function

The driver needs a call back function to that it will call to tell the result of a function call.

An simple example of a call back function can be seen here:
\code
void tsl2591Callback(tsl2591ReturnCode_t rc)
{
	uint16_t _tmp;
	float _lux;
	switch (rc)
	{
	case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl2591GetFullSpectrumRaw(&_tmp)) )
		{
			printf("\nFull Raw:%04X\n", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("\nFull spectrum overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetVisibleRaw(&_tmp)) )
		{
			printf("Visible Raw:%04X\n", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetInfraredRaw(&_tmp)) )
		{
			printf("Infrared Raw:%04X\n", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Infrared overflow - change gain and integration time\n");
		}

		if ( TSL2591_OK == (rc = tsl2591GetLux(&_lux)) )
		{
			printf("Lux: %5.4f\n", _lux);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}
	break;

	case TSL2591_OK:
	// Last command performed successful
	break;

	case TSL2591_DEV_ID_READY:
	// Dev ID now fetched
	break;

	default:
	break;
	}
}
\endcode

Add to application initialization:

Initialise the driver by given it a function pointer to your call back function (in this example: tsl2591Callback):
\code
tsl2591Create(tsl2591Callback)
if ( TSL2591_OK == tsl2591Create(tsl2591Callback) )
{
	// Driver created OK
	// Always check what tsl2591Create() returns
}
\endcode

\section tsl2591_make_measuring How to perform a new measuring with the sensor 

In this use case, the steps to perform a measuring is shown.

\note The driver must be created (see \ref tsl2591_setup_use_case) before a measuring is possible.

The sensor must be powered up before it can be used. This is done with the following command:
\code
if ( TSL2591_OK == tsl2591Enable() )
{
	// The power up command is now send to the sensor - it can be powered down with a call to tsl2591Disable()
}
\endcode

\note The sensor is not powered up before the driver calls the call back function with TSL2591_OK.
It is only necessary to power up the sensor once - or if it has been powered down with a call to tsl2591Disable()

\subsection tsl2591_make_measuring2 Start the measuring
The following must be added to the application code:
\code
	if ( TSL2591_OK != tsl2591FetchData() )
	{
		// Something went wrong
		// Investigate the return code further
	}
	else
	{
		The light data will be ready after the driver calls the call back function with TSL2591_DATA_READY.
	}
\endcode

*/

#endif /* TSL2591_H_ */