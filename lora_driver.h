/**
\file
\brief Driver to MicroChip RN2483 LoRaWAN module

\author Ib Havn

\defgroup lora_driver LoRaWAN Driver using RN2384 Module
\{
\brief LoRaWAN library using MicroChip RN2483 LoRaWAN module.

The implementation works with interrupt, meaning that there are no busy-waiting involved.

\note Dependent on FreeRTOS version 10.x.

See \ref lora_driver_quickstart.

\defgroup lora_creation Functions to initialise the driver.
\brief How to initialise the driver.

\defgroup lora_config LoRa driver configuration functions
\brief How to configure the LoRaWAN driver.

\defgroup lora_basic_function Basic LoRa driver functions
\brief Commonly used LoRaWAN functions.
Here you you will find the functions you normally will need.

\defgroup lora_advanced_function Advanced LoRa driver functions
\brief Advanced LoRaWAN functions.
These functions you will not normally need, it will normally be enough to use the simple functions.

\defgroup lora_driver_return_codes LoRa driver Return codes
\brief Return codes from LoRa driver functions.
\}
*/
#ifndef LORA_DRIVER_H_
#define LORA_DRIVER_H_
#include <stdbool.h>
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>

#include <serial.h>

#define LORA_MAX_PAYLOAD_LENGTH	20 /* bytes - Must newer be changed!!!*/

/**
\ingroup lora_config
\brief Payload data structure.

This is the struct to be used when sending and receiving payload data via the driver.
*/
typedef struct lora_driver_payload {
	uint8_t portNo; /**< Port_no the data is received on, or to transmit to */
	uint8_t len; /**< Length of the payload (no of bytes) */
	uint8_t bytes[LORA_MAX_PAYLOAD_LENGTH]; /**< Array to hold the payload to be sent, or that has been received */
} lora_driver_payload_t;

/**
\ingroup lora_driver_return_codes
\brief LoRA Driver return codes.

These are the codes that can be returned from calls to the driver.
For more documentation of these error codes see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>
*/
typedef enum Lora_driver_returnCodes {
	LORA_OK	/**< Everything went well */
	, LORA_ERROR /**< An error occurred - the reason is not explained any further  */
	, LORA_KEYS_NOT_INIT /**< The necessary keys are not initialized */
	, LORA_NO_FREE_CH	/**< All channels are buzy */
	, LORA_SILENT /**< The module is in a Silent Immediately state */
	, LORA_BUSY /**< The MAC state of the module is not in an idle state */
	, LORA_MAC_PAUSED /**< The MAC is in PAUSED state and needs to be resumed back*/
	, LORA_DENIED /**< The join procedure was unsuccessful (the module attempted to join the
	network, but was rejected) */
	, LORA_ACCEPTED  /**< The join procedure was successful */
	, LORA_INVALID_PARAM  /**< One of the parameters given is wrong */
	, LORA_NOT_JOINED /**< The network is not joined */
	, LORA_INVALID_DATA_LEN /**< If application payload length is greater than the maximum
	application payload length corresponding to the current data rate */
	, LORA_FRAME_COUNTER_ERR_REJOIN_NEEDED /**< If the frame counter rolled over - a rejoin is needed */
	, LORA_MAC_TX_OK /**< If up link transmission was successful and no down link data was
	received back from the server */
	, LORA_MAC_RX /**< If there is a downlink message is received on an uplink transmission */
	, LORA_MAC_ERROR /**< If transmission was unsuccessful, ACK not received back from the
	server */
	, LORA_UNKNOWN /**< An unknown error occurred that is not identified by this driver */
} lora_driver_returnCode_t;

/**
\ingroup lora_config
\brief Join modes.
*/
typedef enum lora_driver_joinModes {
	LORA_OTAA = 0  /**< Join the LoRaWAN network with Over The Air Activation (OTAA) */
	,LORA_ABP /**< Join the LoRaWAN network Activation By Personalization (ABP) */
} lora_driver_joinMode_t;

/**
\ingroup lora_config
\brief Adaptive data rates (ADR) modes.
*/
typedef enum lora_driver_adaptiveDataRateModes {
	LORA_OFF = 0 /**< Set ADR to ON */
	,LORA_ON /**< Set ADR to OFF */
} lora_driver_adaptiveDataRate_t;

/**
\ingroup lora_config
\brief Automatic Reply (AR) modes.
*/
typedef enum lora_driver_automaticReplyModes {
	LORA_AR_ON /**< Set AR to ON */
	,LORA_AR_OFF  /**< Set AR to OFF */
} lora_driver_automaticReplyMode_t;

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Get max payload size in bytes.

The maximum allowed number of bytes that must be sent in the payload!
*/
uint8_t lora_driver_getMaxPayloadSize(void);

/* ======================================================================================================================= */
/**
\ingroup lora_creation
\brief Initialise the LoRa driver..

Initialise the LoRa Driver.

\param[in] comPort to be used for communication with the RN2483 module.
\param[in] downlinkMessageBuffer that will be used to buffer down-link messages received from LoRaWAN.

\note If downlinkMessageBuffer is NULL then no down-link messages can be received
*/
void lora_driver_initialise(serial_comPort_t comPort, MessageBufferHandle_t downlinkMessageBuffer);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Set identifiers and keys for a OTAA join.

To use Over the Air Activation (OTAA) the following information is needed:
|Name | LoRaWAN name | Length |
| :---- | :----: | :---------- |
| Application Identifier | AppEUI  | 16 Hex digits |
| Application Key | AppKey  | 32 Hex digits |
| Device Identifier | DevEUI  | 16 Hex digits |

This function sets besides the identifiers and keys the following parameters in the module:
| LoRaWan Parameter | Value |
| :---------------- | :----: |
| Adaptive Data Rate | ON |

\note This must be called before any join is carried out.
\note These data are being stored in RN2384 module by this function.

\param appEUI Application Identifier
\param appKEY Application Key
\param devEUI Application Key

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setOtaaIdentity(char appEUI[17], char appKEY[33], char devEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Set the driver up to using EU868 standard.

The function sets the following parameters:
| Parameter | Channel  | Param 2 | Note |
|:---- | :------- |:-------- | :------------------ |
| mac rx2 frequency |  | 869525000 | Second receive window frequency |
| mac rx2 data rate | 3 | |Second receive window data rate |
| mac ch drrange | 1 | min range 0 max range 6 | |
| mac ch dcycle | 0-7 | 799 | |
| mac set ch freq | 3 | 867100000 | |
| ^ | 4 | 867300000 | |
| ^ | 5 | 867500000 | |
| ^ | 6 | 867700000 | |
| ^ | 7 | 867900000 | |
| mac ch drrange | 3-7 | min range 0 max range 5 | |
| mac set ch status | 3-7 | on | |
| mac set pwridx | 1 | 1 | The index value for the output power ( 1 = 14 dBm) |

\note This must be called before join with OTAA is carried out.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_configureToEu868(void);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Map a LoRa Driver return code into corresponding text.

\param[in] returnCode to be mapped to corresponding text. \see lora_driver_returnCodes
\return Text representation of return code.
*/
char * lora_driver_mapReturnCodeToText(lora_driver_returnCode_t returnCode);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Set identifiers and keys for a ABP join.

To use Activation By Personalization (ABP) the following information is needed:
|Name | LoRaWAN name | Length |
| :---- | :----: | :---------- |
| Network Session Key | NwkSKey  | 32 Hex digits |
| Application Session Key | AppSKey  | 32 Hex digits |
| Device Address | DevAddr  | 8 Hex digits |

This function sets besides the identifiers and keys the following parameters in the module:
| LoRaWan Parameter | Value |
| :---------------- | :----: |
| Adaptive Data Rate | ON |

\note This must be called before join with ABP is carried out.
\note These data are being stored in RN2384 module by this function.

\param[in] nwkSKEY Network Session Key
\param[in] appSKEY Application Session Key
\param[in] devADD Device Address

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setAbpIdentity(char nwkSKEY[33], char appSKEY[33], char devADD[9]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Joins a LoRaWAN either with ABP or OTAA.

\param[in] mode LORA_OTAA or LORA_ABP

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_join(lora_driver_joinMode_t mode);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Send a upload message to the LoRaWAN.

\param[in] confirmed true: Send confirmed, else unconfirmed.
\param[in] payload pointer to payload to be sent.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_sendUploadMessage(bool confirmed, lora_driver_payload_t * payload);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the device EUI.

\note This is normally done by using \ref lora_driver_setOtaaIdentity.
\note Only needed when OTAA is used.

\param[in] devEUI 16 byte hexadecimal string.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setDeviceIdentifier(const char devEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Application EUI.

\note This is normally done by using \ref lora_driver_setOtaaIdentity.
\note Only needed when OTAA is used.

\param[in] appEUI 16 byte hexadecimal string.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setApplicationIdentifier(const char appEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Application Key.

\note This is normally done by using  \ref lora_driver_setOtaaIdentity.
\note Only needed when OTAA is used.

\param[in] appKey 32 byte hexadecimal string.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setApplicationKey(const char appKey[33]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Network Session Key.

\note This is normally done by using  \ref lora_driver_setAbpIdentity.
\note Only needed when ABP is used.

\param[in] nwkSKey 32 byte hexadecimal string.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setNetworkSessionKey(const char nwkSKey[33]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Application Session Key.

\note This is normally done by using  \ref lora_driver_setAbpIdentity.
\note Only needed when ABP is used.

\param[in] appSKey 32 byte hexadecimal string.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setApplicationSessionKey(const char appSKey[33]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Device address.

\note This is normally done by using  \ref lora_driver_setAbpIdentity.
\note Only needed when ABP is used.

\param[in] devAddr 8 byte hexadecimal string.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setDeviceAddress(const char devAddr[9]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set data rate.

The data rate determines the spreading factor and bit rate on the LoRaWAN. For more information see <a href="https://lora-alliance.org/sites/default/files/2018-04/lorawantm_regional_parameters_v1.1rb_-_final.pdf">LoRaWA Regional Parameters v1.1rB</a>
\param[in] dr [0..7] data rate to be used for next transmissions.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setDataRate(uint8_t dr);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get data rate.

The data rate determines the spreading factor and bit rate on the LoRaWAN. For more information see <a href="https://lora-alliance.org/sites/default/files/2018-04/lorawantm_regional_parameters_v1.1rb_-_final.pdf">LoRaWA Regional Parameters v1.1rB</a>

\ref lora_driver_setDataRate.

\param[out] dr data rate as set in module.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_getDataRate(uint8_t * dr);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set adaptive data rate (ADR).

If ADR is ON the server will optimize the data rate and transmission power based on the last received up-link message.
\param[in] state the wanted ADR state.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setAdaptiveDataRate(lora_driver_adaptiveDataRate_t state);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get adaptive data rate (ADR).

see also \ref lora_driver_setAdaptiveDataRate.

\param[out] state the current state of ADR.
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_getAdaptiveDataRate(lora_driver_adaptiveDataRate_t * state);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the delay between a transmission and the first receiver window.

This command will set the delay between the transmission and the first Reception window to \code rxDelay1 \endcode in milliseconds.
The delay between the transmission and the second Reception window is calculated in software as the delay between the transmission and the first Reception window + 1000 ms.

\param[in] rxDelay1 the delay in ms - default is 1000.
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setReceiveDelay(uint16_t rxDelay1);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set automatic reply on down link messages.

By enabling the automatic reply, the module will transmit a packet without a payload immediately after a confirmed 
downlink message is received, or when the Frame Pending bit has been set by the server. If set to OFF, no automatic reply will be transmitted.

\param[in] ar new state of automatic response.
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setAutomaticReply(lora_driver_automaticReplyMode_t ar);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get automatic reply setting for down link messages.

see also \ref lora_driver_setAutomaticReply.

\param[out] ar current state of automatic response.
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_getAutomaticReply(lora_driver_automaticReplyMode_t * ar);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the delay each link check is performed.

This command sets the time interval for the link check process to be triggered
periodically. A value of '0' will disable the link check process. When the time
interval expires, the next upload message that will be sent to the server will include
also a link check MAC command.

\param[in] sec time between link check is performed [s]. 0: turn off link check.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setLinkCheckInterval(uint16_t sec); // [0..65535]

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get the results of the latest received Link Check.

\see \ref lora_driver_setLinkCheckInterval

This function will return the no of gwy's that was seeing the device at latest upload command, and also the demodulation margin (Margin) in the range of 0..254
indicating the link margin in dB of the last successfully received LinkCheckReq command. A value of '0' means that the frame was received at the demodulation floor (0 dB or no margin)
while a value of '20', for example, means that the frame reached the gateway 20 dB above the demodulation floor.

\param[out] no_gwys that successfully received over last upload message.
\param[out] margin the demodulation margin [dB]
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_getLinkCheckResult(uint8_t * no_gwys, uint8_t * margin);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the spreading factor for the communication.

The spreading factor (SF) ....
\todo spreading factor (SF) needs more explanation!

\param[in] sf spreading factor to be used.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_setSpreadingFactor(uint8_t sf);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Controls the reset pin on the RN2483 Module.


\param[in] state 1: reset is active, 0: reset is released.
*/
void lora_driver_resetRn2483(uint8_t state);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Flush the internal buffers in the driver.

*/
void lora_driver_flushBuffers(void);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Get the RN2483 factory set devEUI.

This device ID is unique in time and space.

This hardware device ID is not automatically being used as the devEUI seen from the LoRaWAN. The later must be set using \ref lora_driver_setOtaaIdentity or \ref lora_driver_setDeviceIdentifier.

\param[out] hwDevEUI buffer where the hardware device ID will be returned.
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_getRn2483Hweui(char hwDevEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Get the RN2483 modules supply voltage VDD.

\todo Implement lora_driver_rn2483GetVdd function!

\param[out] mv buffer where the VDD voltage will be returned [mv]
\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_rn2483GetVdd(char mv[5]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Reset the RN2483 module.

Reboots the module and automatically restores the last saved parameters set in the module.
For a list of restored parameters see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_rn2483Reboot(void);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Reset the RN2483 module.

Reboots the module and restores all parameters to factory settings.
\note I can't find a list of these default values.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_rn2483FactoryReset(void);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the RN2384 module in sleep mode for a given periode.

This command puts the system to Sleep for the specified number of milliseconds. The
module can be forced to exit from Sleep by sending a break condition followed by a
0x55 character at the new baud rate. Note that the break condition needs to be long
enough not to be interpreted as a valid character at the current baud rate.

\todo Implement lora_driver_sleep function.

\note If the module is in sleep mode it will save battery power.

\param[in] ms The number of milliseconds to sleep [100-4294967296].
*/
lora_driver_returnCode_t lora_driver_sleep(uint32_t ms);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Save the set parameters into the EEPROM of the RN2483 module.

For a list of restored parameters see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_saveMac(void);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Pause the MAC layer in the RN2483 module.

This must be done before any commands are send to the radio layer.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_pauseMac(void);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Resume the MAC layer in the RN2483 module.

This must be done after a pause is finished.

\return lora_driver_returnCode
*/
lora_driver_returnCode_t lora_driver_resumeMac(void);

/**
\page lora_driver_quickstart Quick start guide for RN2483 based LoRa Driver

This is the quick start guide for the \ref lora_driver, with
step-by-step instructions on how to configure and use the driver in a
selection of use cases.

The use cases contain several code fragments. The code fragments in the
steps for setup can be copied into a custom initialization function, while
the steps for usage can be copied into, e.g., the main application function.

\section lora_driver_use_cases LoRa Driver use cases
- \ref lora_setup_use_case
- \subpage lora_setup_to_OTAA
- \subpage lora_setup_to_ABP
- \subpage lora_send_uplink_message
- \subpage lora_receive_downlink_message

\section lora_setup_use_case Initialise the driver
The following must be added to the project:
- \code
#include <lora_driver.h>
\endcode

Add to application initialization:
- Initialise the driver without down-link possibility:
\code
lora_driver_initialise(ser_USART1, NULL); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
\endcode
- Alternatively initialise the driver with down-link possibility:
\code
MessageBufferHandle_t downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_payload_t)*2); // Here I make room for two downlink messages in the message buffer
lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
\endcode

Then the LoRaWAN transceiver needs to be hardware reset.
\note This must be done from a FreeRTOS task!!
\code
lora_driver_resetRn2483(1); // Activate reset line
vTaskDelay(2);
lora_driver_resetRn2483(0); // Release reset line
vTaskDelay(150); // Wait for tranceiver module to wake up after reset
lora_driver_flushBuffers(); // get rid of first version string from module after reset!
\endcode

Now you are ready to use the LoRaWAN module :)
*/

/**
\page lora_setup_to_OTAA OTAA setup steps
\note All the following code must be implemented in the initialisation part of a FreeRTOS task!
\note Nearly all calls to the driver will suspend the calling task while the driver waits for response from the RN2484 module.


\section lora_basic_use_case_setupOTAA_code Example code
In this use case, the driver is setup to Over The Air Activation (OTAA).

\section lora_setup_use_case_OTAA_setup_flow Workflow
-# Define the necessary app identification for OTAA join:
\code
// Parameters for OTAA join
#define LORA_appEUI "????????????????"
#define LORA_appKEY "????????????????????????????????"
\endcode

\note The parameters depends on the setup of the LoRaWAN network server and will be given to you.

-# Set the module to factory set defaults:
\code
if (lora_driver_rn2483FactoryReset() != LORA_OK)
{
	// Something went wrong
}
\endcode

-# Configure the module to use the EU868 frequency plan and settings:
\code 
if (lora_driver_configureToEu868() != LORA_OK)
{
	// Something went wrong
}
\endcode

-# Get the RN2483 modules unique devEUI:
\code
static char devEui[17]; // It is static to avoid it to occupy stack space in the task
if (lora_driver_getRn2483Hweui(devEui) != LORA_OK)
{
	// Something went wrong
}
\endcode

-# Set the necessary LoRaWAN parameters for an OTAA join:
\code 
if (lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,devEui) != LORA_OK)
{
	// Something went wrong
}
\endcode

-# Save all set parameters to the RN2483 modules EEPROM (OPTIONAL STEP):
\note If this step is performed then it is no necessary to do the steps above more than once. These parameters will automatically be restored in the module on next reset or power on.

\code
if (lora_driver_saveMac() != LORA_OK)
{
	// Something went wrong
}

// All parameters are now saved in the module
\endcode

-# Join LoRaWAN parameters with OTAA:
\code 
if (lora_driver_join(LORA_OTAA) == LORA_ACCEPTED)
{
	// You are now joined
}
 \endcode
*/

/**
 \page lora_setup_to_ABP ABP setup steps
 \note All the following code must be implemented in the initialisation part of a FreeRTOS task!
 \note Nearly all calls to the driver will suspend the calling task while the driver waits for response from the RN2484 module.

 \section lora_basic_use_case_setup_ABP_code Example code
 In this use case, the driver is setup to Activation by personalization (ABP).

 \section lora_setup_use_case_ABP_setup_flow Workflow
 -# Define the necessary app identification for ABP join:
 \code
// Parameters for ABP join
#define LORA_appAddr "????????"
#define LORA_nwkskey "????????????????????????????????"
#define LORA_appskey "????????????????????????????????"
 \endcode
  
 \note The parameters depends on the setup of the LoRaWAN network server and will be given to you.

 -# Set the module to factory set defaults:
 \code
 if (lora_driver_rn2483FactoryReset() != LORA_OK)
 {
	 // Something went wrong
 }
 \endcode

 -# Configure the module to use the EU868 frequency plan and settings:
 \code
 if (lora_driver_configureToEu868() != LORA_OK)
 {
	 // Something went wrong
 }
 \endcode

 -# Set the necessary LoRaWAN parameters for an ABP join:
 \code
 if (lora_driver_setAbpIdentity(LORA_nwkskey,LORA_appskey,LORA_appAddr) != LORA_OK)
 {
	 // Something went wrong
 }
 \endcode

 -# Save all set parameters to the RN2483 modules EEPROM (OPTIONAL STEP):
 \note If this step is performed then it is no necessary to do the steps above more than once. These parameters will automatically be restored in the module on next reset or power on.

 \code
 if (lora_driver_saveMac() != LORA_OK)
 {
	 // Something went wrong
 }

 // All parameters are now saved in the module
 \endcode

 -# Join LoRaWAN parameters with ABP:
 \code
 if (lora_driver_join(LORA_ABP) == LORA_ACCEPTED)
 {
	 // You are now joined
 }
 \endcode
*/

/**
\page lora_send_uplink_message Sent an uplink message

In this use case, an uplink message will be send.

\note The driver must be initialised \ref lora_setup_use_case and must be setup to OTAA \ref lora_setup_to_OTAA or ABP \ref lora_setup_to_OTAA.

In this example these two variables will be send in an uplink message
\par
\code
	uint16_t hum; // Humidity
	int16_t temp; // Temperature
\endcode

\section lora_send_uplink_message_setup Uplink Message Setup
The following must be added to a FreeRTOS task in the project:
-# Define a payload struct variable
\code
	lora_driver_payload_t uplinkPayload;
\endcode

-# Populate the payload struct with data
\code 
	uplinkPayload.len = 4; // Length of the actual payload
	uplinkPayload.port_no = 1; // The LoRaWAN port no to sent the message to

	uplinkPayload.bytes[0] = hum >> 8;
	uplinkPayload.bytes[1] = hum & 0xFF;
	uplinkPayload.bytes[2] = temp >> 8;
	uplinkPayload.bytes[3] = temp & 0xFF;
 \endcode
-# Send the uplink message:
\code 
	lora_driver_returnCode_t rc;

	if ((rc = lora_driver_sendUploadMessage(false, &_uplinkPayload)) == LORA_MAC_TX_OK )
	{
		// The uplink message is sent and there is no downlink message received
	}
	else if (rc == LORA_MAC_RX_OK)
	{
		// The uplink message is sent and a downlink message is received
	}
 \endcode
 */

 /**
\page lora_receive_downlink_message Receive a downlink message

In this use case, a downlink link message will be received.
A downlink message is received in a lora_payload_t variable.

\note The driver must be initialised \ref lora_setup_use_case and must be setup to OTAA \ref lora_setup_to_OTAA or ABP \ref lora_setup_to_OTAA.
\note To be able to receive any downlink messages you may specify a FreeRTOS message buffer during the initialisation of the driver. In this message buffer the received messages will be delivered by the driver (\ref lora_setup_use_case).

In this example a downlink message with 4 bytes will be received from the LoRaWAN.
These 4 bytes will in this example represent a maximum humidity setting and a maximum temperature setting that we want to be able to recieve from the LoRaWAN.
\par
\code
	uint16_t maxHumSetting; // Max Humidity
	int16_t maxTempSetting; // Max Temperature
\endcode

\section lora_receive_downlink_message_setup Down-link Message Setup

The following must be added to a FreeRTOS tasks for(;;) loop in your application - typical you will have a separate task for handling downlink messages:
-# Define a payload struct variable
Create a payload variable to receive the down-link message in
\code
	lora_driver_payload_t downlinkPayload;
\endcode

-# Wait for a message to be received
\code 
	// this code must be in the loop of a FreeRTOS task!
	xMessageBufferReceive(downLinkMessageBufferHandle, &downlinkPayload, sizeof(lora_payload_t), portMAX_DELAY);
	printf("DOWN LINK: from port: %d with %d bytes received!", downlinkPayload.port_no, downlinkPayload.len); // Just for Debug
	if (4 == downlinkPayload.len) // Check that we have got the expected 4 bytes
	{
		// decode the payload into our variales
		maxHumSetting = (downlinkPayload.bytes[0] << 8) + downlinkPayload.bytes[1];
		maxTempSetting = (downlinkPayload.bytes[2] << 8) + downlinkPayload.bytes[3];
	}
 \endcode
*/
#endif /* LORA_DRIVER_H_ */