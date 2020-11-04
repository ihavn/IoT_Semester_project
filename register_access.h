#pragma once
#include <stdint.h>

// GPIO Macros
#define PORT2DDR(port)	(*(volatile uint8_t *)((&port)-1))
#define PORT2PIN(port)	(*(volatile uint8_t *)((&port)-2))

uint8_t register_access_readRegister(volatile uint8_t * reg);
uint8_t register_access_readBit(volatile uint8_t * reg, uint8_t bitno);
uint8_t register_access_readBits(volatile uint8_t * reg, uint8_t mask);

void register_access_setRegister(volatile uint8_t * reg, uint8_t value);
void register_access_setBit(volatile uint8_t * reg, uint8_t bitno);
void register_access_setBits(volatile uint8_t * reg, uint8_t mask);

void register_access_toggleBit(volatile uint8_t * reg, uint8_t bitno);
void register_access_toggleBits(volatile uint8_t * reg, uint8_t mask);

void register_access_clearRegister(volatile uint8_t * reg);
void register_access_clearBit(volatile uint8_t * reg, uint8_t bitno);
void register_access_clearBits(volatile uint8_t * reg, uint8_t mask);