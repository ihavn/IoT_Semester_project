# Hardware documentation
## Datasheets for Sensor and Hardware Modules
Documentation for used sensors and hardware modules [Datasheets for Sensor and Hardware Modules](DATASHEETS.md)

## Documentation for the PCBs designed and created at VIA
![VIA Hardware](/docs/resources/IoT-Hardware.jpg)
### VIA MEGA 2560 Shield
[Schematic](/docs/documentation/HW%20Doc/VIA%20MEGA2560%20Shield/VIA%20Shield%202.0.0%20Schematic.pdf)

[Board Layout](/docs/documentation/HW%20Doc/VIA%20MEGA2560%20Shield/VIA%20Shield%202.0.0%20Board.pdf)

[MCU Pin Usage](/docs/documentation/HW%20Doc/VIA%20MEGA2560%20Shield/MCU-Pin%20Usage.pdf)

#### USB and JTAG connections
To power the hardware and to communicate with it a USB cable must be connected to your computer (shown with red arrow).

For programming and debugging the MCU the ATMEL ICE Debugger module must be connected to the JTAG connector on the ARDUINO MEGA 2560 Shield (also shown with red arrow)

![USB Connection](/docs/resources/IoT-Hardware%20Connections.jpg)

The JTAG connector from the ATMEL ICE debugger must be connected like shown in this picture

![JTAG Connection](/docs/resources/IoT-Hardware%20JTAG.jpg)

### VIA Micro Click Host Board
[Schematic](/docs/documentation/HW%20Doc/Mikro%20Click%20Host%20Board/Mikro%20Click%20Host%20Board%20Schematic.pdf)

[Board Layout](/docs/documentation/HW%20Doc/Mikro%20Click%20Host%20Board/Mikro%20Click%20Host%20Board.pdf)
#### Jumper Setting
It is **VERY important** to check that the jumpers are mounted like shown with red arrows in this picture

![Host Board Jumper Settings](/docs/resources/Host%20Board%20Jumper.jpg)

The *LoRa click* module **MUST** be mounted in the socket marked *3V3!*

JP1 **MUST** be mounted on the two pins to the right marked *3V3*!

JP2 **MUST** be mounted on the two pins to the left marked *5V*! 

### VIA IoT Sensor Board
[Schematic](/docs/documentation/HW%20Doc/IoT%20Sensor%20Board/Sensor%20connection%20Schematic.pdf)

[Board Layout](/docs/documentation/HW%20Doc/IoT%20Sensor%20Board/Sensor%20connection%20board.pdf)
