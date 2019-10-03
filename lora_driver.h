/**
\file

\author Ib Havn
\version 1.0.0

\defgroup lora_driver LoRaWAN Driver using RN2384 Module
\{
\brief LoRaWAN library using MicroChip RN2483 LoRaWAN module.

The implementation works with interrupt, meaning that there are no busy-waiting involved.

\note Dependent on FreeRTOS version 10.x.

See \ref lora_driver_quickstart.

\defgroup lora_creation Functions to create and initialize the driver.
\brief How to create the driver.

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
#include <serial.h>

//#include <lorawan_config.h>

#define LORA_MAX_PAYLOAD_LENGTH	20 /* bytes - Must newer be changed!!!*/

/**
\ingroup lora_config
\brief Payload data structure.

This is the struct to be used when sending and receiving payload data via the driver.
*/
typedef struct lora_payload {
	uint8_t port_no; /**< Port_no the data is received on, or to transmit to */
	uint8_t len; /**< Length of the payload (no of bytes) */
	uint8_t bytes[LORA_MAX_PAYLOAD_LENGTH]; /**< Array to hold the payload to be sent, or that has been received */
} lora_payload_t;

/**
\ingroup lora_driver_return_codes
\brief LoRA Driver return codes.

These are the codes that can be returned from calls to the driver.
For more documentation of these error codes see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>
*/
typedef enum LoRa_driver_return_codes {
	LoRA_OK	/**< Everything went well */
	, LoRa_ERROR /**< An error occurred - the reason is not explained any further  */
	, LoRa_KEYS_NOT_INIT /**< The necessary keys are not initialized */
	, LoRa_NO_FREE_CH	/**< All channels are buzy */
	, LoRa_SILENT /**< The module is in a Silent Immediately state */
	, LoRa_BUSY /**< The MAC state of the module is not in an idle state */
	, LoRa_MAC_PAUSED /**< The MAC is in PAUSED state and needs to be resumed back*/
	, LoRa_DENIED /**< The join procedure was unsuccessful (the module attempted to join the
	network, but was rejected) */
	, LoRa_ACCEPTED  /**< The join procedure was successful */
	, LoRa_INVALID_PARAM  /**< One of the parameters given is wrong */
	, LoRa_NOT_JOINED /**< The network is not joined */
	, LoRa_INVALID_DATA_LEN /**< If application payload length is greater than the maximum
	application payload length corresponding to the current data rate */
	, LoRa_FRAME_COUNTER_ERR_REJOIN_NEEDED /**< If the frame counter rolled over - a rejoin is needed */
	, LoRa_MAC_TX_OK /**< If up link transmission was successful and no down link data was
	received back from the server */
	, LoRa_MAC_RX_OK /**< If there is a downlink message is received on an uplink transmission */
	, LoRa_MAC_ERROR /**< If transmission was unsuccessful, ACK not received back from the
	server */
	, LoRa_UNKNOWN /**< An unknown error occurred that is not identified by this driver */
} e_LoRa_return_code_t;

/**
\ingroup lora_config
\brief Join modes.
*/
typedef enum lora_join_modes {
	LoRa_OTAA = 0  /**< Join the LoRaWAN network with Over The Air Activation (OTAA) */
	,LoRa_ABP /**< Join the LoRaWAN network Activation By Personalization (ABP) */
} e_join_mode_t;

/**
\ingroup lora_config
\brief Adaptive data rates (ADR) modes.
*/
typedef enum lora_adaptive_data_rate_modes {
	LoRa_OFF = 0 /**< Set ADR to ON */
	,LoRa_ON /**< Set ADR to OFF */
} e_LoRa_adaptive_data_rate_t;

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Get max payload size in bytes.

The maximum allowed number of bytes that must be sent in the payload!
*/
uint8_t lora_driver_get_max_payload_size(void);

/* ======================================================================================================================= */
/**
\ingroup lora_creation
\brief Create the LoRa driver..

Creates and initialize the LoRa Driver.

\param[in] com_port to be used for communication with the RN2483 module.
*/
void lora_driver_create(e_com_port_t com_port);

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
| Spreading Factor | 7 |

\note This must be called before any join is carried out.
\note These data are being stored in RN2384 module by this function.

\param appEUI Application Identifier
\param appKEY Application Key
\param devEUI Application Key

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_otaa_identity(char appEUI[17], char appKEY[33], char devEUI[17]);

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

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_configure_to_eu868(void);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Map a LoRa Driver return code into corresponding text.

\param return_code to be mapped to corresponding text. \see LoRa_driver_return_codes
\return Text representation of return code.
*/
char * lora_driver_map_return_code_to_text(e_LoRa_return_code_t return_code);

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
| Spreading Factor | 7 |

\note This must be called before join with ABP is carried out.
\note These data are being stored in RN2384 module by this function.

\param nwkSKEY Network Session Key
\param appSKEY Application Session Key
\param devADD Device Address

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_abp_identity(char nwkSKEY[33], char appSKEY[33], char devADD[9]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Joins a LoRaWAN either with ABP or OTAA.

\param mode LoRa_OTAA or LoRa_ABP

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_join(e_join_mode_t mode);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Sent a upload message to the LoRaWAN.

\param[in] confirmed true: Send confirmed, else unconfirmed.
\param[in] payload pointer to payload to be sent.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_sent_upload_message(bool confirmed, lora_payload_t * payload);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the device EUI.

\note This is normally done by using \ref lora_driver_set_otaa_identity.
\note Only needed when OTAA is used.

\param[in] devEUI 16 byte hexadecimal string.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_device_identifier(const char devEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Application EUI.

\note This is normally done by using \ref lora_driver_set_otaa_identity.
\note Only needed when OTAA is used.

\param[in] appEUI 16 byte hexadecimal string.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_application_identifier(const char appEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Application Key.

\note This is normally done by using  \ref lora_driver_set_otaa_identity.
\note Only needed when OTAA is used.

\param[in] appKey 32 byte hexadecimal string.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_application_key(const char appKey[33]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Network Session Key.

\note This is normally done by using  \ref lora_driver_set_abp_identity.
\note Only needed when ABP is used.

\param[in] nwkSKey 32 byte hexadecimal string.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_network_session_key(const char nwkSKey[33]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Application Session Key.

\note This is normally done by using  \ref lora_driver_set_abp_identity.
\note Only needed when ABP is used.

\param[in] appSKey 32 byte hexadecimal string.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_application_session_key(const char appSKey[33]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the Device address.

\note This is normally done by using  \ref lora_driver_set_abp_identity.
\note Only needed when ABP is used.

\param[in] devAddr 8 byte hexadecimal string.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_device_address(const char devAddr[9]);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set data rate.

The data rate determines the spreading factor and bit rate on the LoRaWAN. For more information see <a href="https://lora-alliance.org/sites/default/files/2018-04/lorawantm_regional_parameters_v1.1rb_-_final.pdf">LoRaWA Regional Parameters v1.1rB</a>
\param[in] dr [0..7] data rate to be used for next transmissions.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_data_rate(uint8_t dr);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get data rate.

The data rate determines the spreading factor and bit rate on the LoRaWAN. For more information see <a href="https://lora-alliance.org/sites/default/files/2018-04/lorawantm_regional_parameters_v1.1rb_-_final.pdf">LoRaWA Regional Parameters v1.1rB</a>

\ref lora_driver_set_data_rate.

\param[out] dr data rate as set in module.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_get_data_rate(uint8_t * dr);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set adaptive data rate (ADR).

If ADR is ON the server will optimize the data rate and transmission power based on the last received up-link message.
\param[in] state the wanted ADR state.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_adaptive_data_rate(e_LoRa_adaptive_data_rate_t state);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get adaptive data rate (ADR).

see also \ref lora_driver_set_adaptive_data_rate.

\param[out] state the current state of ADR.
\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_get_adaptive_data_rate(e_LoRa_adaptive_data_rate_t * state);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the delay between a transmission and the first receiver window.

This command will set the delay between the transmission and the first Reception window to \code rxDelay1 \endcode in milliseconds.
The delay between the transmission and the second Reception window is calculated in software as the delay between the transmission and the first Reception window + 1000 ms.

\param[in] rxDelay1 the delay in ms - default is 1000.
\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_receive_delay(uint16_t rxDelay1);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the delay each link check is performed.

This command sets the time interval for the link check process to be triggered
periodically. A value of '0' will disable the link check process. When the time
interval expires, the next upload message that will be sent to the server will include
also a link check MAC command.

\param[in] sec time between link check is performed [s]. 0: turn off link check.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_setLink_check_interval(uint16_t sec); // [0..65535]

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Get the results of the latest received Link Check.

\see \ref lora_driver_setLink_check_interval

This function will return the no of gwy's that was seeing the device at latest upload command, and also the demodulation margin (Margin) in the range of 0..254
indicating the link margin in dB of the last successfully received LinkCheckReq command. A value of '0' means that the frame was received at the demodulation floor (0 dB or no margin)
while a value of '20', for example, means that the frame reached the gateway 20 dB above the demodulation floor.

\param[out] no_gwys that successfully received over last upload message.
\param[out] margin the demodulation margin [dB]
\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_get_link_check_result(uint8_t * no_gwys, uint8_t * margin);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the spreading factor for the communication.

The spreading factor (SF) ....
\todo spreading factor (SF) needs more explanation!

\param[in] sf spreading factor to be used.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_set_spreading_factor(uint8_t sf);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Controls the reset pin on the RN2483 Module.


\param[in] state 1: reset is active, 0: reset is released.
*/
void lora_driver_reset_rn2483(uint8_t state);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Flush the internal buffers in the driver.

*/
void lora_driver_flush_buffers(void);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Get the RN2483 factory set devEUI.

This device ID is unique in time and space.

This hardware device ID is not automatically being used as the devEUI seen from the LoRaWAN. The later must be set using \ref lora_driver_set_otaa_identity or \ref lora_driver_set_device_identifier.

\param[out] hwDevEUI buffer where the hardware device ID will be returned.
\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_get_rn2483_hweui(char hwDevEUI[17]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Get the RN2483 modules supply voltage VDD.

\param[out] mv buffer where the VDD voltage will be returned [mv]
\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_rn2483_get_vdd(char mv[5]);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Reset the RN2483 module.

Reboots the module and automatically restores the last saved parameters set in the module.
For a list of restored parameters see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_rn2483_reset(void);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Reset the RN2483 module.

Reboots the module and restores all parameters to factory settings.
\note I can't find a list of these default values.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_rn2483_factory_reset(void);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Set the RN2384 module in sleep mode for a given periode.

This command puts the system to Sleep for the specified number of milliseconds. The
module can be forced to exit from Sleep by sending a break condition followed by a
0x55 character at the new baud rate. Note that the break condition needs to be long
enough not to be interpreted as a valid character at the current baud rate.

\todo Implement wakeup function.

\note If the module is in sleep mode it will save battery power.

\param[in] ms The number of milliseconds to sleep [100-4294967296].
*/
e_LoRa_return_code_t lora_driver_sleep(uint32_t ms);

/* ======================================================================================================================= */
/**
\ingroup lora_basic_function
\brief Save the set parameters into the EEPROM of the RN2483 module.

For a list of restored parameters see <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf">RN2483 LoRa Technology Module
Command Reference User's Guide</a>

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_save_mac(void);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Pause the MAC layer in the RN2483 module.

This must be done before any commands are send to the radio layer.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_pause_mac(void);

/* ======================================================================================================================= */
/**
\ingroup lora_advanced_function
\brief Resume the MAC layer in the RN2483 module.

This must be done after a pause is finished.

\return eLoRa_return_code
*/
e_LoRa_return_code_t lora_driver_resume_mac(void);

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

\section lora_setup_use_case Initialise the driver
The following must be added to the project:
- \code
#include <ihal.h>
#include <lora_driver.h>
\endcode

Add to application initialization:
- Initialise the driver:
\code
hal_create(LED_TASK_PRIORITY); // Must be called first!! LED_TASK_PRIORITY must be a high priority in your system
lora_driver_create(ser_USART1); // The parameter is the USART port the RN2483 module is connected to - in this case USART1
\endcode

Then the LoRaWAN transceiver needs to be hardware reset.
\note This must be done from a FreeRTOS task!!
\code
lora_driver_reset_rn2483(1); // Activate reset line
vTaskDelay(2);
lora_driver_reset_rn2483(0); // Release reset line
vTaskDelay(150); // Wait for tranceiver module to wake up after reset
lora_driver_flush_buffers(); // get rid of first version string from module after reset!
\endcode

Now you are ready to use the LoRaWAN module :)
*/

/**
\page lora_setup_to_OTAA OTAA setup steps
\note All the following code must be implemented in the initialisation part of a FreeRTOS task!
\note Nearly all calls to the driver will suspend the calling task while the driver waits for response from the RN2484 module.

\subsection lora_basic_use_case_setupOTAA_code Example code
In this use case, the driver is setup to Over The Air Activation (OTAA).

\subsection lora_setup_use_case_OTAA_setup_flow Workflow
-# Define the necessary app identification for OTAA join:
\code
// Parameters for OTAA join
#define LORA_appEUI "E81068FC10812076"
#define LORA_appKEY "3894B87078D8A38B56E419ABCA16043E"
\endcode

-# Set the module to factory set defaults:
\code
if (lora_driver_rn2483_factory_reset() != LoRA_OK)
{
	// Something went wrong
}
\endcode

-# Configure the module to use the EU868 frequence plan and settings:
\code 
if (lora_driver_configure_to_eu868() != LoRA_OK)
{
	// Something went wrong
}
\endcode

-# Get the RN2483 modules unique devEUI:
\code
static char dev_eui[17]; // It is static to avoid it to occupy stack space in the task
if (lora_driver_get_rn2483_hweui(dev_eui); != LoRA_OK)
{
	// Something went wrong
}
\endcode

-# Set the necessary LoRaWAN parameters for an OTAA join:
\code 
if (lora_driver_set_otaa_identity(LORA_appEUI,LORA_appKEY,dev_eui) != LoRA_OK)
{
	// Something went wrong
}
\endcode

-# Save all set parameters to the RN2483 modules EEPROM (OPTIONAL STEP):
\note If this step is performed then it is no necessary to do the steps above more than once. These parameters will automatically be restored in the module on next reset or power on.

\code
if (lora_driver_save_mac() != LoRA_OK)
{
	// Something went wrong
}

// All parameters are now saved in the module
\endcode

-# Join LoRaWAN parameters with OTAA:
\code 
if (lora_driver_join(LoRa_OTAA) == LoRa_ACCEPTED)
{
	// You are now joined
}
 \endcode
*/

/**
 \page lora_setup_to_ABP ABP setup steps
 \note All the following code must be implemented in the initialisation part of a FreeRTOS task!
 \note Nearly all calls to the driver will suspend the calling task while the driver waits for response from the RN2484 module.

 \subsection lora_basic_use_case_setup_ABP_code Example code
 In this use case, the driver is setup to Activation by personalization (ABP).

 \subsection lora_setup_use_case_ABP_setup_flow Workflow
 -# Define the necessary app identification for ABP join:
 \code
// Parameters for ABP join
#define LORA_appAddr "02AF853A"
#define LORA_nwkskey "B9C26F5B1E717D7A73A48DB4B928774F"
#define LORA_appskey "86AAC887A5F891A1A442CD0085A6E44B"
 \endcode

 -# Set the module to factory set defaults:
 \code
 if (lora_driver_rn2483_factory_reset() != LoRA_OK)
 {
	 // Something went wrong
 }
 \endcode

 -# Configure the module to use the EU868 frequence plan and settings:
 \code
 if (lora_driver_configure_to_eu868() != LoRA_OK)
 {
	 // Something went wrong
 }
 \endcode

 -# Set the necessary LoRaWAN parameters for an ABP join:
 \code
 if (lora_driver_set_abp_identity(LORA_nwkskey,LORA_appskey,LORA_appAddr) != LoRA_OK)
 {
	 // Something went wrong
 }
 \endcode

 -# Save all set parameters to the RN2483 modules EEPROM (OPTIONAL STEP):
 \note If this step is performed then it is no necessary to do the steps above more than once. These parameters will automatically be restored in the module on next reset or power on.

 \code
 if (lora_driver_save_mac() != LoRA_OK)
 {
	 // Something went wrong
 }

 // All parameters are now saved in the module
 \endcode

 -# Join LoRaWAN parameters with ABP:
 \code
 if (lora_driver_join(LoRa_ABP) == LoRa_ACCEPTED)
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

\subsection lora_send_uplink_message_setup create a payload variable
The following must be added to a FreeRTOS task in the project:
-# Define a payload struct variable
\code
	lora_payload_t uplink_payload;
\endcode
\subsection lora_send_uplink_message_flow Workflow
-# Populate the payload struct with data
\code 
	uplink_payload.len = 4; // Length of the actual payload
	uplink_payload.port_no = 1; // The LoRaWAN port no to sent the message to

	uplink_payload.bytes[0] = hum >> 8;
	uplink_payload.bytes[1] = hum & 0xFF;
	uplink_payload.bytes[2] = temp >> 8;
	uplink_payload.bytes[3] = temp & 0xFF;
 \endcode
-# Send the uplink message:
\code 
	e_LoRa_return_code_t rc;

	if ((rc = lora_driver_sent_upload_message(false, &_uplink_payload)) == LoRa_MAC_TX_OK )
	{
		// The uplink message is sent and there is no downlink message received
	}
	else if (rc == LoRa_MAC_RX_OK)
	{
		// The uplink message is sent and a downlink message is received
	}
 \endcode
*/
#endif /* LORA_DRIVER_H_ */