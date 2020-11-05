/*
 * hal_defs.h
 *
 * Created: 17/10/2018 10:38:37
 *  Author: IHA
 */ 


#ifndef HAL_DEFS_H_
#define HAL_DEFS_H_

#include <serial.h>

// Lora Transceiver
#define LORA_USART	ser_USART1
#define LORA_RESET_PORT PORTK
#define LORA_RESET_PIN PK4

// MH-Z19 CO2 Driver
#define MH_Z19_USART ser_USART3
 
#endif /* HAL_DEFS_H_ */