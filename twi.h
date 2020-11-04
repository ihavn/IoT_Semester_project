/** \file
	\todo Document twi.h
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <stdbool.h>

// Abstract Data Type (ADT)
typedef struct twiStruct *twi_p;

typedef enum {twi400kHz, twi100kHz} twiClock_t;
typedef enum {
	TWI_OK = 0x01							// Command OK
	,TWI_BUSY = 0x02						// TWI-Bus is busy
	,TWI_FREE = 0x03						// TWI-Bus is available
	,TWI_TRANSMIT_OK = 0x04					// Transmit completed OK
	,TWI_RECEIVE_OK = 0x05					// Receive completed OK
	,TWI_NULL_HANDLER = 0x06				// NULL given as handler
	,TWI_MESSAGE_TO_LONG = 0x07				// Message to long for internal buffers - can be extended in twi.c define
	,TWI_BUS_ERROR = 0x00					// TW-Bus error due to illegal START or STOP condition
	,TWI_MASTER_TX_ADR_WRITE_NACK = 0x20	// Transmission of SLA+W has received NACK
	,TWI_MASTER_TX_DATA_NACK = 0x30			// Transmission of DATA has received NACK
	,TWI_MASTER_RX_ADR_READ_NACK = 0x48		// Transmission of SLA+R has received NACK
} twiReturnCode_t;

twi_p twiCreate(uint8_t slaveAddr, twiClock_t speed, uint32_t f_cpu, void(*handlerCallback)(twi_p, twiReturnCode_t, uint8_t *, uint8_t));
void twiDestroy(twi_p twiHandler);

bool twiIsBusy( void );
twiReturnCode_t twiTransmit(twi_p twiHandler, uint8_t * bytes, uint8_t noOfBytes );
twiReturnCode_t twiReceive(twi_p twiHandler, uint8_t noOfBytes );

#endif /* TWI_H_ */