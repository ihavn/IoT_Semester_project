#pragma once
#include <stdint.h>
uint8_t reg_read_register(volatile uint8_t * reg);
uint8_t reg_read_bit(volatile uint8_t * reg, uint8_t bitno);
uint8_t reg_read_bits(volatile uint8_t * reg, uint8_t mask);

void reg_set_register(volatile uint8_t * reg, uint8_t value);
void reg_set_bit(volatile uint8_t * reg, uint8_t bitno);
void reg_set_bits(volatile uint8_t * reg, uint8_t mask);

void reg_toggle_bit(volatile uint8_t * reg, uint8_t bitno);
void reg_toggle_bits(volatile uint8_t * reg, uint8_t mask);

void reg_clear_register(volatile uint8_t * reg);
void reg_clear_bit(volatile uint8_t * reg, uint8_t bitno);
void reg_clear_bits(volatile uint8_t * reg, uint8_t mask);