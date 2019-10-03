/**
\file

\author Ib Havn
\version 1.0.0

\note This driver is dependent on FreeRTOS, and will create a FreeRTOS task.

\defgroup led_driver Driver for Status Leds
\{
\brief Driver to control the four status leds on the <I>VIA ARDUINO MEGA2560 Shield rev. 2.0</I>.

\defgroup led_driver_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup led_driver_basic_function Basic driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.
\}
*/
#ifndef ILED_H_
#define ILED_H_

#include "ihal.h"

/**
\ingroup led_driver_basic_function
\brief Led identifications.

These are the enumerations that must be used when manipulating with the status leds.
*/
typedef enum {
	led_ST1 = 0		/**< Status led ST1 (RED)*/
	,led_ST2		/**< Status led ST2 (GREEN)*/
	,led_ST3		/**< Status led ST3 (YELLOW)*/
	,led_ST4		/**< Status led ST4 (BLUE)*/
} eleds_t;

/* ======================================================================================================================= */
/**
\ingroup led_driver_creation
\brief Create the led driver.

Creates and initialize the driver.

\note The priority must be pretty high to make the leds behave smoothly.

\param[in] *register_func pointer to the register manipulation functions that will be injected.
\param[in] led_task_priority the priority the drivers internal task will be using.
*/
void led_create(hal_register_access_t * register_func, UBaseType_t led_task_priority);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led light up in a long period.

\param[in] led to use.
*/
void led_long_puls(eleds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led light up in a short period.

\param[in] led to use.
*/
void led_short_puls(eleds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led blink with a low frequency.

\param[in] led to use.
*/
void led_slow_blink(eleds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Makes the led blink with a high frequency.

\param[in] led to use.
*/
void led_fast_blink(eleds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Turns the led on.

\param[in] led to use.
*/
void led_led_on(eleds_t led);

/* ======================================================================================================================= */
/**
\ingroup led_driver_basic_function
\brief Turns the led off.

\param[in] led to use.
*/
void led_led_off(eleds_t led);

#endif /* ILED_H_ */