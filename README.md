# NanoESP32-BW-RGBW-clock
One PCB, one source code for 2835/3528 BW and SK6812/WS2812 RGB(W) LED-strips
![PCB-ESP32V002-3D](https://github.com/user-attachments/assets/5af4b536-eb22-4bef-80e0-2ab3298e05da)

The first word clock was designed with a ATMEGA328, combined with a rotary, DCF77 receiver, a accurate Ds3231 RTC klok module and a Bluetooth module.
The design was greatly simplified with the use of Arduino Nano and WS2812 and SK6812 RGB colour LEDs.

with the Arduino Nano and WS2812 and SK6812 RGB color leds the design became a lot simpler. 

The arrival of the ESP32 was superior to the Arduino mcu's with WIFI and bluetooth working both simutaniously. But with too many short living boards with different foorprints, sizes and, often to less, pins no suitable satisfying design could be made.

The Arduino Nano ESP32 with S3 processor was the promising Arduino product. 

Maintaining the source code for the White LED and RGB LED clocks was a time-consuming task. The white LED clock, which has been running in numerous clocks for over 10 years, has been built by many people and proved to be very solid.

The RGB(W) clock made it possible to nuilt a 4-language clock with 625 LEDs. The one language designs are much easier to built and can also give a pure white display and if wished also in any colour.

This is the ultimate fusion of all clocks; one PCB and one source code.<br>
The PCB's were kindly supported by [PCBWay](https://www.pcbway.com) that made the development of this PCB possible.<br>
The PCB was designed with Fritzing and the Gerber files uploaded to PCBWay. Delivery to The Netherlands in a week. Piece of cake.<br>

In the [PDF](https://github.com/ednieuw/NanoESP32-BW-RGBW-clock/blob/main/NanoESP32-PCB.pdf) the solder instructions are documented.

# Components
1 x PCB Nano ESP32
1 x Arduino Nano ESP32
1 x 1000 µF condensator 
3 x 10 µF capacitor
2 x diode 1N5817
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
1 x AMS1117 5V -> 3V3 Step-Down Regulator Module
1 x  12V -> 5V  voltage regulator module
1 x USB cable 5V --> 12V DC
1 x 6-pin female connector (DS3231 RTC)
1 x 5-pin female connector (Rotary)
1 x 3-pin female connector (DCF77)
1 x 2-pin female connector (LDR)
1 x pinheader 40 pins right-angled (2st PCB 5*10cm, 1st PCB 10*10cm)
3 x 9-pin Dupont female connector  (per 3 stuks)
1 x  KY-040 Keyes Rotary Encoder
DCF77 DCF-2 module
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
![image](https://github.com/user-attachments/assets/45d0078e-9c4d-48fe-b7f6-ec2818abd143)
