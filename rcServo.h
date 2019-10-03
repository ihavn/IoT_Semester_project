/**
\file

\author Ib Havn
\version 1.0.0

\defgroup rc_servo_driver Driver for RC-Servos
\{
\brief Driver to control the two RC-Servos that can be connected to the <I>VIA ARDUINO MEGA2560 Shield rev. 2.0</I>.

The RC-Servos can be connected to J13 (Servo#0) and J14 (Servo#1) on the <I>VIA ARDUINO MEGA2560 Shield rev. 2.0</I>.

<b>J13/J14 to Servo connections</b>
| J13/J14 pin | Servo Cable Wire Color |
| :----:| :------------------------------: |
| 1 | Brown |
| 2 | Red |
| 3 | Orange |

\defgroup rc_servo_creation Functions to create and initialize the driver.
\brief How to create the driver.

\defgroup rc_servo_basic_function Basic driver functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.
\}
*/
#ifndef RCSERVO_H_
#define RCSERVO_H_
#include <stdint.h>
/* ======================================================================================================================= */
/**
\ingroup rc_servo_creation
\brief Create the RC-Servo driver.

Creates and initialize the driver.

This should only be called once during initialisation of the application.
*/
void rcServoCreate(void);

/* ======================================================================================================================= */
/**
\ingroup rc_servo_basic_function
\brief Turn the RC-Servo to the specified position.

| Percent | Servo Position |
| :----: | :----- |
| -100 | Fully to the left |
| 0 | Middle position |
| 100 | Fully to the right |

Percents in between will set the servo accordantly to the percentage.

\param[in] servoNo The servo to set the position for (0:J13 (Servo#0) 1:J14 (Servo#1))
\param[in] percent The position to set the servo to [-100 .. 100]
*/
void rcServoSet(uint8_t servoNo, int8_t percent);

#endif /* RCSERVO_H_ */