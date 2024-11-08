# NanoESP32-BW-RGBW-clock

Page under construction. Aug-Oct 2024. PCB and software are working.
Latest PCB ESP32 Woordklok V004

One PCB, one source code for 2835/3528 BW and SK6812/WS2812 RGB(W) LED-strips that can be used to built [word clocks](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock) and Fibonacci clocks.

![image](https://github.com/user-attachments/assets/a9c8ec87-1b29-44bb-b9ab-6a396af67191)
The PCB with 24 white LEDs connected to the shift registers and a 16x16 RGB(W) LED matrix connected working simultaniously with the Nano ESP32

![PCB-ESP32V002-3D](https://github.com/user-attachments/assets/5af4b536-eb22-4bef-80e0-2ab3298e05da)

The [first word clock](https://ednieuw.home.xs4all.nl/Woordklok/Bouwpakket/WoordklokWitteLEDs.html) was [designed](https://github.com/ednieuw/Woordklok-witte-LEDs) with a ATMEGA328, combined with a rotary, DCF77 receiver, an accurate DS3231 RTC klok module and a Bluetooth module.
The electronic design was greatly simplified with the use of Arduino Nano Every and WS2812 RGB or SK6812 RGBW colour LEDs.

The arrival of the ESP32 was superior to the Arduino MCU's with WIFI and Bluetooth working both simultaneously. But with too many short living boards with different footprints, sizes and, often to less, pins a suitable satisfying design could not be made.

The Arduino Nano ESP32 with S3 processor was the promising Arduino product. 

Maintaining the source code for the White LED and RGB LED clocks was a time-consuming task. The white LED clock, which has been running in numerous clocks for over 10 years, has been built by many people and proved to be a very solid design wit LEDs operating for more than 10 years.

The RGB(W) clock made it possible to built a 4-language clock with 625 LEDs. The designs with one language in a 12 x 12 letter grid are much easier to built and the SK6812 RGBW LEDs can also give a pure white display and, if wished, also in any colour.

This is the ultimate fusion of all clocks; one PCB and one source code.<br> 
This board is mainly designed for the 12V white LED-strips with an Arduino Nano ESP32. Adding the RGB(W) functionality was minor and now with this PCB both LED types can be used. 
[A PCB for only RGB(W) LED-strips](https://github.com/ednieuw/NanoESP32PCB).

The several versions of this PCB design were kindly supported by [PCBWay](https://www.pcbway.com) that made the development of this PCB possible.<br>
The PCB was designed with Fritzing and the Gerber files uploaded to  the PCBWay web site.<br>
Delivery to The Netherlands was within a week. Just upload the [Gerber files from the ZIP file](https://github.com/ednieuw/NanoESP32PCB) to their site and pay the charges. Piece of cake.<br>
The boards looked perfect and soldering the components was easy.

In this [PDF](https://github.com/ednieuw/NanoESP32-BW-RGBW-clock/blob/main/NanoESP32-PCB.pdf) the solder instructions are documented.
![SolderedUltimatePCB](https://github.com/user-attachments/assets/5755a473-0637-4cd6-abb2-cd6641e7e1b3)


[PCB in Action](https://github.com/user-attachments/assets/d6f986bc-7fbc-4ec3-888b-1f40599507ed) <br>
All LEDs functioning.

![PCBUltimateV005](https://github.com/user-attachments/assets/5e62c2aa-8987-44ea-9be9-f6130198b4a5)

Download the [latest PCB Fritzing and gerber files here](https://github.com/ednieuw/NanoESP32PCB)

# Connections on the PCB.
The software support the following connected devices.

- A One-wire connection to pin A3.   See here [One wire 1x3 and 3x4 membrane keypad](https://ednieuw.home.xs4all.nl/Woordklok/OneWireKeyPad/OneWireKeyPad.html)
- A 6-pin connection to SDA and SCL pin A4 and A5 for a DS3231 RTC module. When no WIFI is available the software can use this very precise RTC module instead of the NTP time. The SDA and SCL line can be optionally pulled-up with two optional 10k resitors.
- A 3-pin connection  connected to D8 for a DCF-77 time module to get the time from a in Mainflingen (Germany) located time transmitter. [More here](https://ednieuw.home.xs4all.nl/Woordklok/DCF77_transceiver/DCFtransceiverklok.html). 
- A 5-pin connection for a rotary encoder.
- A 3-pin connection to connect a SK6812 RGBW 0r WS2812 RGB LED strip
Fixed connection from pins A0, A1 and A2 to the three HC595 shift registers that turns on and off the ULN2803 IC's that switch on and off 12V 2835/3528 and many more white LED strips.
- A 2-pin connection from LDR to A6
- Pin A7 gives a PWM signal to the two transistors. The BC327 gives a 12V PWM signal to the white LEDs with a maximum of 800 mA. This is enough for a 50 cm large Word clock with 3 meters of 2835 LED-strip. (not all strips are on the same time) if more power is needed heavier transistors can be used or 24 relays can be turned on and off  
- A 2-pin connection to D11
- A 3 pin connector connected via a 470 ohm resistor to Pin D5 through the level shifter is used to drive the RGB(W) LED-strip
- The 74AHCT125 level shifter shifts the data signals from pin D5, D6 and D7 from 3V3 to 5V. 
- Two 2-pin connectors for 5V signals from pin D6 and D7 through the level shifter.
- D10 and D12 connect to the right and left LED respectively.
- All pins can also be accessed from beside the Nano ESP32 connector.

An AMS1117 step down voltage regulator an can be fitted to have more power on the 3V3 pins instead of the 3V3 from the Arduino.
On top of the PCB a 78L05 voltage regulator can be used to convert 12V to 5 V to drive the 5V part of the PCB.
At the bottom a 12V to 5V voltage regulator can be fitted instead of the 78L05 voltage regulator.

It is also possible to use the 5V of the ESP32 Nano when 12V is connected to Vin.
Altertively 5V can be drawn from the USB port when a USB-C cable is connected to the ESP32.
With jumpers on the PCB the different power options can be chosen.
At last a 5V - 12V power supply can be used and connected to the 5V-GND-12V connector at the bottom of the PCB.

For white LED clock only a 12V connection to one of the two 12V power connectors is needed. Connect the 12V to Vin with a jumper.<br>
It is also possible to use a USB 5V to 12V cable. These are cheap and powerfull enough for the clock. Even with a simple phone power supply. 

# For the colour clocks it is advisable to power the LED-strip separate from the PCB. Then do not connect the 5V to the LED-strip but only GND and the Data line on D5<br>
Up to 100 LEDs can be powered from the 5V power connector on the PCB. But keep in mind the current has to flow throught the PCB and thos thin lines can melt if overloaded.<br> 
It is safer to feed the strip at several positions with 5V and connect at least one GND to the GND of the PCB

# Components
In [PDF](https://github.com/ednieuw/NanoESP32-BW-RGBW-clock/blob/main/NanoESP32-PCB.pdf) the solder instructions are documented.
```

1 x PCB Nano ESP32
1 x Arduino Nano ESP32
1 x 1000 µF condensator 
3 x 10 µF capacitor
2 x diode 1N5817 (One on PCB version V002)
3 x 470Ω resistor
6 x 10kΩ resistor
1 x 4.7kΩ resistor
1 x BC327 transistor
1 x BC547 transistor
1 x LED red
1 x LED yellow
3 x 74HC595 DIP16 socket
3 x ULN2803APG  DIP18 socket
3 x 74HC595 8-bit shift register DIP16
3 x ULN2803APG voltage regulator DIP18
1 x 78L05/ L7805 voltage regulator 5V, 1.5A
1 x cooling element voltage regulator
1 x 2-pin female connector (LDR)
1 x 3-pin female connector (DCF77)
1 x 5-pin female connector (Rotary)
1 x 6-pin female connector (DS3231 RTC)
2 x 15-pin female connector
1 x AMS1117 5V -> 3V3 Step-Down Regulator
1 x 12V -> 5V  voltage regulator module
1 x USB cable 5V --> 12V DC
1 x 6-pin female connector (DS3231 RTC)
1 x 5-pin female connector (Rotary)
1 x 3-pin female connector (DCF77)
1 x 2-pin female connector (LDR)
1 x pinheader 40 pins right-angled
1 x pinheader 6 pins straight with 3 x shortcut connectors
3 x 9-pin Dupont female connector
1 x KY-040 Keyes Rotary Encoder
1 x DCF77 DCF-2 module
1 x 4x3 or 1x3 membrane keypad
1 x RCT DS3231 Precisie klok module ZS-042
1 x CR 2032 3V lithium battery
1 x LDR light sensor 
1 x USB cable 5V --> 12V DC
1 x power connector 2.54mm
1 x power connector KF350 5.08 mm
1 x power connector XH2.54
1 x WS2812 LED-strip 
1 x SK6812 LED-strip 
1 x 2 meter Cool White SMD 2835/3528, 120 LEDS/meter, LED-strip 12V
```
