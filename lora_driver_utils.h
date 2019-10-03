/*
 * lora_driver_utils.h
 *
 * Created: 05/11/2018 15:31:41
 *  Author: IHA
 */ 


#ifndef LORA_DRIVER_UTILS_H_
#define LORA_DRIVER_UTILS_H_

#define NIBLE_TO_HEX_CHAR(nibble) ((nibble) > 9 ? 55 + (nibble) : '0' + (nibble))

uint8_t decode_port_no(const char * message, uint8_t * decode_pos);
void decode_hexadecimal_string_bytes(uint8_t bytes[], uint8_t size_of_bytes, const char * hex_string);

#endif /* LORA_DRIVER_UTILS_H_ */