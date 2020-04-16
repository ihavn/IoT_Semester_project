/*
 * hal_defs.h
 *
 * Created: 17/10/2018 10:38:37
 *  Author: IHA
 */ 


#ifndef HAL_DEFS_H_
#define HAL_DEFS_H_

#include <serial.h>

// GPIO Macros
#define PORT2DDR(port)	(*(volatile uint8_t *)((&port)-1))
#define PORT2PIN(port)	(*(volatile uint8_t *)((&port)-2))

// Status Leds
#define ST1_LED_PORT  PORTC
#define ST1_LED_PIN PC6
#define ST1_LED_ACTIVE_LEVEL 0

#define ST2_LED_PORT PORTC
#define ST2_LED_PIN PC7
#define ST2_LED_ACTIVE_LEVEL 0

#define ST3_LED_PORT PORTG
#define ST3_LED_PIN PG5
#define ST3_LED_ACTIVE_LEVEL 0

#define ST4_LED_PORT PORTH
#define ST4_LED_PIN PH4
#define ST4_LED_ACTIVE_LEVEL 0

// Lora Transceiver
#define LORA_USART	ser_USART1
#define LORA_RESET_PORT PORTK
#define LORA_RESET_PIN PK4

// MH-Z19 CO2 Driver
#define MH_Z19_USART ser_USART3
 


#endif /* HAL_DEFS_H_ */