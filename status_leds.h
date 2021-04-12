/**
\file
\brief Driver to the four Status leds that are found on the <I>VIA ARDUINO MEGA2560 Shield rev. 2.0</I>

\author Ib Havn
\version 2.0.0

\note This driver is dependent on FreeRTOS, and will create a FreeRTOS task.

\defgroup led_driver Driver for Status Leds
\{
\brief Driver to control the four status leds on the <I>VIA ARDUINO MEGA2560 Shield rev. 2.0</I>.

\defgroup led_driver_creation Functions to initialize the driver.
\brief How to initialise the driver.

\defgroup led_driver_basic_function Basic driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.
\}
*/
#ifndef STATUS_LEDS_H_
#define STATUS_LEDS_H_

/**
\ingroup led_driver_basic_function
\brief Led identifications.

These are the enumerations that must be used when manipulating with the status LEDs.
*/
typedef enum {
	led_ST1 = 0		/**< Status led ST1 (RED)*/
	,led_ST2		/**< Status led ST2 (GREEN)*/
	,led_ST3		/**< Status led ST3 (YELLOW)*/
	,led_ST4		/**< Status led ST4 (BLUE)*/
} status_leds_t;

/* ======================================================================================================================= */
/**
\ingroup led_driver_creation
\brief Initialise the led driver.

Initialise the driver.

\note The priority must be pretty high to make the LEDs behave smoothly.

\param[in] ledTaskPriority the priority the drivers internal task will be using.
*/
void status_leds_initialise(UBaseType_t ledTaskPriority);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led light up in a long period.

\param[in] led to use.
*/
void status_leds_longPuls(status_leds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led light up in a short period.

\param[in] led to use.
*/
void status_leds_shortPuls(status_leds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led blink with a low frequency.

\param[in] led to use.
*/
void status_leds_slowBlink(status_leds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led blink with a high frequency.

\param[in] led to use.
*/
void status_leds_fastBlink(status_leds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Turns the led on.

\param[in] led to use.
*/
void status_leds_ledOn(status_leds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Turns the led off.

\param[in] led to use.
*/
void status_leds_ledOff(status_leds_t led);

#endif /* STATUS_LEDS_H_ */