/*
 * hal.h
 *
 * Created: 17/10/2018 11:35:58
 *  Author: IHA
 */ 


#ifndef HAL_H_
#define HAL_H_

#include <avr/io.h>

// Function pointers to functions in IRegisterAccess
typedef struct {
	uint8_t (* read_register)(volatile uint8_t * reg);
	uint8_t (* read_bit)(volatile uint8_t * reg, uint8_t bitno);
	uint8_t (* read_bits)(volatile uint8_t * reg, uint8_t mask);

	void (* set_register)(volatile uint8_t * reg, uint8_t value);
	void (* set_bit)(volatile uint8_t * reg, uint8_t bitno);
	void (* set_bits)(volatile uint8_t * reg, uint8_t mask);

	void (* toggle_bit)(volatile uint8_t * reg, uint8_t bitno);
	void (* toggle_bits)(volatile uint8_t * reg, uint8_t mask);

	void (* clear_register)(volatile uint8_t * reg);
	void (* clear_bit)(volatile uint8_t * reg, uint8_t bitno);
	void (* clear_bits)(volatile uint8_t * reg, uint8_t mask);
} hal_register_access_t;

 void hal_create(uint8_t led_task_priority);

#endif /* HAL_H_ */