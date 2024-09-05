/* 
 Author .    : Ed Nieuwenhuys
 Changes V001: Derived from ESP32Arduino_WordClockV035.ino
 Changes V002: SK6812 LEDs working. LDR pin A2 to A6
 Changes V003: HC595coding op pin A0 A1 A2 
 Changes V004: Time function upgraded. Removed RTCLib and added wire commands
 Changes V005: Moved BW and RGB clock defines to TABs
 Changes V006: No errors when compiling with UVBW, NLBW, NL114CLOCK
 Changes V007: No errors in FOURLANGUAGECLOCK
 Changes V008: Rotary, one-wire buttons and DCF77 added, RTC working, DCF77 NOT, One-wire working??
 Changes V009: Clean up. Optimized code and function name and location. 
               Rotary does not place the time in the DS3231   
               DCF77 does not function well with time functions. Try later
 Changes V010: When RGB clock is chosen HC595 shift one place every second. If RGB clock then it can be used to control something
 Changes V011: Clean up. 
 Changes V012: Added WIFI and NTP events. Added in Loop() NTPsyncEvent();
 Changes V013: Ulrich's clock face added #define NLM1M2M3M4L94 #define NLM1M2M3M4L256 and >18 to >19 in SayHour of Ulrich's clock.Clock faces identical as in ESP32ArduinoWordclockV036   
 Changes V014: Renamed to ESP32WordClockUltimatePCB-V014. All clock faces compile without errors
 Changes V015: Added   <meta name="google" content="notranslate" /> in Webpage.h
 Changes V016: Coding moved to relevant header files. Source compile for all clock selections        
 Changes V017: Reverted back to V015. The RGBW LED functions should remain in primary source code. M1M2M3M4 in BW clock. BWUV clock to Ulrich Vertessen for testing 
 Changes V018: Compiles with board esp32-> Arduino Nano ESP32 -> By GPIO number.  Added MIN0; to clear the four RGB(W) LEDs       
 Changes V019: Removed Colour display choice from menu with BW clocks
 Changes V020: Removed command WiFi.mode(WIFI_STA); 
 Changes V021: DS3231 use tested and status can be seen in menu. Corrected 2000 year mismatch in DS3231
 Changes V022: 

              To Do:Check one wire and if Rotary places the time in the DS3231.  

*** Special designed for ESP32 Ultimate PCB with shift registers and RGB(W) support *** 

How to compile: 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
Check if ELEGANTOTA_USE_ASYNC_WEBSERVER 1 in ElegantOTA.h
Locate the ELEGANTOTA_USE_ASYNC_WEBSERVER macro in the ElegantOTA.h file, and set it to 1: -->  #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1

Install Arduino Nano ESP32 boards
Board: Arduino Nano ESP32
Partition Scheme: With FAT
Pin Numbering: By GPIO number (legacy)

Select below, with ONLY ONE #define selected, the clock type
Upload the file to the Arduino Nano ESP32 installed on PCB "Nano ESP32 Word clock"

*/
// =============================================================================================================================
//--------------------------------------------
// ESP32 Definition of preferred word clock type
// Define the clock type by removing the // before the #define
//--------------------------------------------

// ------------------>   Define ONLY ONE clock type!

#define NL144CLOCK            // RGB(W) Standard 12x12 clock
//#define FOURLANGUAGECLOCK     // RGB(W) 4-languages clock

//#define NLM1M2M3M4L94         // NL clock with four extra LEDs for the minutes to light up for Ulrich
//#define NLM1M2M3M4L256         // NL clock with four extra LEDs for the minutes to light up for Ulrich 
 
//#define NLBW                 // NL White LEDs clock with HC595 Shift bit registers for control
// #define UKBW               // UK White LEDs clock with HC595 Shift bit registers for control
//#define UVBW               // Ulrich White LEDs clock with HC595 Shift bit registers for control

//--------------------------------------------
// ESP32 Definition of installed modules
// Define the modules installed in the clock by removing the // before the #define
//--------------------------------------------
// #define ROTARYMOD            // Use a rotary encoder
// #define ONEWIREKEYPAD3x1       // Use a 3x1 keypad with one wire
// #define ONEWIREKEYPAD3x4     // Use a 3x4 keypad with one wire

// #define DCF77MOD             // DCF77 installed.Does not work with NTPtme. Remove NTP time functionality to use the DCF77

//--------------------------------------------
// ESP32 Includes defines and initialisations
//--------------------------------------------

#include <Preferences.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel   for LED strip WS2812 or SK6812
#include <NimBLEDevice.h>      // For BLE communication  https://github.com/h2zero/NimBLE-Arduino
#include <ESPNtpClient.h>      // https://github.com/gmag11/ESPNtpClient
#include <WiFi.h>              // Used for web page 
#include <AsyncTCP.h>          // Used for webpage  https://github.com/dvarrel/AsyncTCP                    old https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncWebServer.h> // Used for webpage  https://github.com/mathieucarbou/ESPAsyncWebServer     Old one with  https://github.com/me-no-dev/ESPAsyncWebServer
#include <ElegantOTA.h>        // If ElegantOTA compile errors occur see here :https://docs.elegantota.pro/async-mode
                               // Locate the ELEGANTOTA_USE_ASYNC_WEBSERVER macro in the ElegantOTA.h file, and set it to 1:
                               // #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
#include <Wire.h>              // Used for I2C 

                      #ifdef ROTARYMOD
#include <Encoder.h>
                      #endif //ROTARYMOD
                      #ifdef DCF77MOD
#include <DCF77.h>
#include <TimeLib.h>
                      #endif //DCF77MOD


                      #if defined(FOURLANGUAGECLOCK) || defined(NL144CLOCK)  || defined(NLM1M2M3M4L256) || defined( NLM1M2M3M4L94)
#define RGBCLOCK
                      #endif
                      #if defined(NLBW) || defined(UKBW)  || defined(UVBW)  
#define BWCLOCK
                      #endif
//--------------------------------------------
// SPIFFS storage
//--------------------------------------------
Preferences FLASHSTOR;

//--------------------------------------------
// COLOURS stored in Mem.DisplayChoice
//--------------------------------------------   
const byte DEFAULTCOLOUR = 0;
const byte HOURLYCOLOUR  = 1;          
const byte WHITECOLOR    = 2;
const byte OWNCOLOUR     = 3;
const byte OWNHETISCLR   = 4;
const byte WHEELCOLOR    = 5;
const byte DIGITAL       = 6;
const byte ANALOOG       = 7;

//------------------------------------------------------------------------------
// PIN Assigments for Arduino Nano ESP32
//------------------------------------------------------------------------------ 
 
enum DigitalPinAssignments {      // Digital hardware constants
 SERRX        = D0,               // D0 Connects to TX
 SERTX        = D1,               // D1 Connects to RX
 encoderPinB  = D2,               // D2 left (labeled CLK on decoder)no interrupt pin  
 encoderPinA  = D3,               // D3 right (labeled DT on decoder)on interrupt  pin
 clearButton  = D4,               // D4 switch (labeled SW on decoder)
 LED_PIN      = D5,               // D5 Pin to control colour SK6812/WS2812 LEDs
 PIN_D6       = D6,               // D6 To Level shifter 74AHCT125 when Connected
 PIN_D7       = D7,               // D7 To Level shifter 74AHCT125 when Connected
 DCF_PIN      = D8,               // D8 DCF pin
 DCF_LED_Pin  = D9,               // D9 Optional for DCF_LED 
 EMPTYD9      = D9,               // D9 EMPTY
 PCB_LED_D10  = D10,              // D10 Right LED on PCB ESP32 Word clock V003
 PIN_D11      = D11,              // D11 Connector on PCB
 PCB_LED_D12  = D12,              // D12 Left LED
 secondsPin   = D13               // D13 GPIO48 Right LED on PCB V001 & V002 (#ifdef LED_BUILTIN  #undef LED_BUILTIN #define LED_BUILTIN 48 #endif)
 };
 
enum AnaloguePinAssignments {     // Analogue hardware constants ----
 LEDDataPin   = A0,               // HC595      D17
 LEDStrobePin = A1,               // HC595      D18
 LEDClockPin  = A2,               // HC595      D19
 OneWirePin   = A3,               // OneWirePin D20
 SDA_pin      = A4,               // SDA pin    D21
 SCL_pin      = A5,               // SCL pin    D22
 PhotoCellPin = A6,               // LDR pin    D23
 PWMpin       = A7                // Pin that controls PWM signal on BC327 transistor to dim light D24
};
//--------------------------------------------                                                //
const uint32_t black    = 0x000000, darkorange    = 0xFF8C00, red        = 0xFF0000, chartreuse = 0x7FFF00;
const uint32_t brown    = 0x503000, cyberyellow   = 0xFFD300, orange     = 0xFF8000; 
const uint32_t yellow   = 0xFFFF00, cadmiumyellow = 0xFFF600, dyellow    = 0xFFAA00, chromeyellow = 0xFFA700;
const uint32_t green    = 0x00FF00, brightgreen   = 0x66FF00, apple      = 0x80FF00, grass  = 0x00FF80;  
const uint32_t amaranth = 0xE52B50, edamaranth    = 0xFF0050, amber      = 0xFF7E00;
const uint32_t marine   = 0x0080FF, darkviolet    = 0x800080, pink       = 0xFF0080, purple = 0xFF00FF; 
const uint32_t blue     = 0x0000FF, cerulean      = 0x007BA7, sky        = 0x00FFFF, capri  = 0x00BFFF;
const uint32_t edviolet = 0X7500BC, frenchviolet  = 0X8806CE, coquelicot = 0xFF3800;
const uint32_t greenblue= 0x00F2A0, hotmagenta    = 0xFF00BF, dodgerblue = 0x0073FF, screamingreen = 0x70FF70;
      uint32_t white    = 0xFFFFFF, lgray         = 0x666666;
      uint32_t gray     = 0x333333, dgray         = 0x222222;  

//--------------------------------------------                                                //
// LED
//--------------------------------------------
extern const uint32_t NUM_LEDS;
const byte BRIGHTNESS    =  16;                                                               // Initial brightness of the strips at startup BRIGHTNESS 0 - 255

Adafruit_NeoPixel LEDstrip;                                                                   // Common define in which LEDn812strip is copied
Adafruit_NeoPixel LED6812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800); 
Adafruit_NeoPixel LED2812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); 

uint32_t MINColor      = chromeyellow;
uint32_t SECColor      = chromeyellow;  
                                             #ifdef FOURLANGUAGECLOCK
uint32_t DefaultColor  = white;   
uint32_t LetterColor   = white; 
uint32_t UKLetterColor = green;   
uint32_t DELetterColor = red;
uint32_t FRLetterColor = yellow;
uint32_t UKDefaultColor= UKLetterColor;
uint32_t DEDefaultColor= DELetterColor;
uint32_t FRDefaultColor= FRLetterColor;
                                             #else
uint32_t DefaultColor  = chromeyellow;   
uint32_t LetterColor   = chromeyellow;   
                                             #endif //FOURLANGUAGECLOCK

uint32_t OwnColour     = 0X002345DD;        // Blueish
uint32_t WhiteColour   = white;
uint32_t WheelColor    = blue;
uint32_t HourColor[] ={  white,      darkviolet, cyberyellow, capri,         amber,         apple,
                         darkorange, cerulean,   edviolet,    cadmiumyellow, green,         edamaranth,
                         red,        yellow,     coquelicot,  pink,          apple,         hotmagenta,
                         green,      greenblue,  brightgreen, dodgerblue,    screamingreen, blue,
                         white,      darkviolet, chromeyellow};     

// Definition of the digits 0 - 9, 3 wide, 5 high for the digital clock display in 12x12 clocks
// 10111
// 10101
// 11101 turn your head 90 degrees left and you see a TWO 
const byte PROGMEM Getal[10][3][5]  = { 
                     { {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1} },  //0
                     { {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1} },  //1
                     { {1, 0, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 0, 1} },  //2
                     { {1, 0, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} },  //3
                     { {1, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1} },  //4
                     { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 1} },  //5
//                   { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 1} },  //5 An other 5 font
                     { {1, 1, 1, 1, 1}, {0, 0, 1, 0, 1}, {0, 0, 1, 1, 1} },  //6
                     { {1, 1, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1} },  //7
                     { {1, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} },  //8
                     { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} }   //9
                     }; 

//--------------------------------------------                                                //
// DS3231 CLOCK MODULE
//--------------------------------------------

#define DS3231_I2C_ADDRESS          0x68
#define DS3231_TEMPERATURE_MSB      0x11
#define DS3231_TEMPERATURE_LSB      0x12
//                                  #ifdef MOD_DS3231
RTC_DS3231 RTCklok; 
DateTime Inow;
byte DS3231Installed = 0;                                                                     // True if the DS3231 is detected

//--------------------------------------------                                                //
// KY-040 ROTARY
//--------------------------------------------
                          #ifdef ROTARYMOD                         
Encoder myEnc(encoderPinA, encoderPinB);                                                      // Use digital pin  for encoder
                          #endif  //ROTARYMOD      
long     Looptime          = 0;
byte     RotaryPress       = 0;                                                               // Keeps track of display choice and how often the rotary is pressed.
uint32_t RotaryPressTimer  = 0;
byte     NoofRotaryPressed = 0;

//--------------------------------------------                                                //
// LDR PHOTOCELL
//--------------------------------------------
//                                                                                            //
const byte SLOPEBRIGHTNESS    = 80;                                                           // Steepness of with luminosity of the LED increases
const int  MAXBRIGHTNESS      = 255;                                                          // Maximun value in bits  for luminosity of the LEDs (1 - 255)
const byte LOWBRIGHTNESS      = 5;                                                            // Lower limit in bits of Brightness ( 0 - 255)   
byte       TestLDR            = 0;                                                            // If true LDR inf0 is printed every second in serial monitor
int        OutPhotocell;                                                                      // stores reading of photocell;
int        MinPhotocell       = 999;                                                          // stores minimum reading of photocell;
int        MaxPhotocell       = 1;                                                            // stores maximum reading of photocell;
uint32_t   SumLDRreadshour    = 0;
uint32_t   NoofLDRreadshour   = 0;

//--------------------------------------------                                                //
// DCF-2 DCF77 MODULE  #include <DCF77.h> 
//--------------------------------------------
byte DCF_signal = 50;                                                                         // Is a proper time received?
byte DCF_counts = 0;                                                                          // Valid DCF receptions per hour
bool SeeDCFsignalInDisplay = false;                                                           // If ON then the display line HET IS WAS will show the DCF77-signal received
bool UseDCF                = true;                                                            // Use the DCF-receiver or not
                                  #ifdef DCF77MOD                                             // DCF77 ------------------------------
#define DCF_INTERRUPT 0                                                                       // DCF Interrupt number associated with DCF_PIN
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT,LOW);
                                  #endif //DCF77MOD  

//--------------------------------------------                                                //
// CLOCK initialysations
//--------------------------------------------                                 

static uint32_t msTick;                                                                       // Number of millisecond ticks since we last incremented the second counter
byte      lastminute = 0, lasthour = 0, lastday = 0, sayhour = 0;
byte      Display1 = 0 , Display2 = 0, Display3 = 0;
bool      Demo                 = false;
bool      Zelftest             = false;
bool      Is                   = true;                                                        // toggle of displaying Is or Was
bool      ZegUur               = true;                                                        // Say or not say Uur in NL clock
struct    tm timeinfo;                                                                        // Storage of time used in clock for time display

//--------------------------------------------                                                //
// BLE   #include <NimBLEDevice.h>
//--------------------------------------------
BLEServer *pServer      = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected    = false;
bool oldDeviceConnected = false;
std::string ReceivedMessageBLE;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"                         // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


//--------------------------------------------                                                //
// WEBSERVER #include <ESPAsyncWebServer.h> 
//--------------------------------------------
int WIFIConnected = 0;                                                                        // Is wIFI connected?
AsyncWebServer server(80);                                                                    // For OTA Over the air uploading
#include "Webpage.h"

//--------------------------------------------                                                //
// NTP #include <ESPNtpClient.h>
//--------------------------------------------
boolean syncEventTriggered = false;                                                           // True if a time even has been triggered
NTPEvent_t ntpEvent;                                                                          // Last triggered event

//--------------------------------------------                                                //
// One-wire keypad
//--------------------------------------------
 bool     ChangeTime           = false;
 bool     ChangeLightIntensity = false;
 bool     KeyInputactivated    = false;
 uint64_t KeyLooptime          = 0;
 String   KeypadString         ="";
//--------------------------------------------                                                //
// Common
//--------------------------------------------
#define MAXTEXT 255
bool      SerialConnected      = true;   
bool      LEDsAreOff           = false;                                                       // If true LEDs are off except time display
bool      NoTextInLeds         = false;                                                       // Flag to control printing of the text in function ColorLeds() and ShiftInTime()
byte      Toggle_HetWasIs      = 1;  
char      sptext[MAXTEXT];                                                                    // For common print use 
int       Previous_LDR_read    = 512;                                                         // The actual reading from the LDR + 4x this value /5
int       ToggleEdsoft         = 1;
uint16_t  MilliSecondValue     = 10;                                                          // The duration of a second  minus 1 ms. Used in Demo mode
uint64_t  Loopcounter          = 0;

struct   EEPROMstorage {                                                                      // Data storage in EEPROM to maintain them after power loss
  byte DisplayChoice    = 0;
  byte TurnOffLEDsAtHH  = 0;
  byte TurnOnLEDsAtHH   = 0;
  byte LanguageChoice   = 0;
  byte LightReducer     = 0;
  int  LowerBrightness  = 0;
  int  UpperBrightness  = 0;
  int  NVRAMmem[24];                                                                          // LDR readings
  byte BLEOn            = 1;
  byte NTPOn            = 1;
  byte WIFIOn           = 1;  
  byte StatusLEDOn      = 1;
  int  ReconnectWIFI    = 0;                                                                  // No of times WIFI reconnected 
  byte DCF77On          = 0;
  byte WIFINoOfRestarts = 0;                                                                  // If 1 than resart MCU once
  byte UseDS3231        = 0;                                                                  // Use the DS3231 time module 
  byte LEDstrip         = 0;                                                                  // 0 = SK6812 LED strip. 1 = WS2812 LED strip,  2 = White LEDs
  byte ByteFuture3      = 0;                                                                  // For future use
  byte ByteFuture4      = 0;                                                                  // For future use
  int  IntFuture1       = 0;                                                                  // For future use
  int  IntFuture2       = 0;                                                                  // For future use
  int  IntFuture3       = 0;                                                                  // For future use
  int  IntFuture4       = 0;                                                                  // For future use   
  byte UseBLELongString = 0;                                                                  // Send strings longer than 20 bytes per message. Possible in IOS app BLEserial Pro 
  uint32_t OwnColour    = 0;                                                                  // Self defined colour for clock display
  uint32_t DimmedLetter = 0;
  uint32_t BackGround   = 0;
  char Ssid[30];                                                                              // 
  char Password[40];                                                                          // 
  char BLEbroadcastName[30];                                                                  // Name of the BLE beacon
  char Timezone[50];
  int  Checksum        = 0;
}  Mem; 
//--------------------------------------------                                                //
// Menu
//0        1         2         3         4
//1234567890123456789012345678901234567890----  
 char menu[][40] = {
 "A SSID B Password C BLE beacon name",
 "D Date (D15012021) T Time (T132145)",
 "E Timezone  (E<-02>2 or E<+01>-1)",
 "F Own colour  (Hex FWWRRGGBB)",
 "I To print this Info menu",
 "J Use DS3231 RTC toggle On/Off",
                       #ifdef DCF77MOD
 "JJ Toggle use DCF77 receiver",
                       #endif //DCF77MOD
 "K LDR reads/sec toggle On/Off", 
 "M Demo mode (msec) (M200)",
 "N Display off between Nhhhh (N2208)",
 "O Display toggle On/Off",
 "P Status LED toggle On/Off", 
                       #ifdef RGBCLOCK
 "Q Display colour choice",
 "  Q0 Yellow  Q1 Hourly  Q2 White",
 "  Q3 All Own Q4 Own     Q5 Wheel",
                      #endif //RGBCLOCK
                      #ifdef NL144CLOCK
 "  Q6 Digital",
                      #endif //NL144CLOCK                      
 "R Reset settings @ = Reset MCU",
 "--Light intensity settings (1-250)--",
 "S=Slope V=Min  U=Max   (S80 V5 U200)",
 "W=WIFI  X=NTP& Y=BLE  Z=Fast BLE",
 "# Self test, ! See RTC, & Update RTC",
 "Ed Nieuwenhuys August 2024" };
 

                      #ifdef RGBCLOCK
#include "RGBWclock.h"
                      #endif
                      #ifdef BWCLOCK  
#include "BWclock.h"
                      #endif


//  -------------------------------------   End Definitions  ---------------------------------------

//--------------------------------------------                                                //
// ARDUINO Setup
//--------------------------------------------
void setup() 
{
 Serial.begin(115200);                                                                        // Setup the serial port to 115200 baud //
 pinMode(secondsPin,   OUTPUT);                                                               // turn On seconds LED-pin
 pinMode(LED_RED,      OUTPUT);
 pinMode(LED_GREEN,    OUTPUT);
 pinMode(LED_BLUE,     OUTPUT);
 pinMode(PCB_LED_D10,  OUTPUT);                                                               // LED on PCB board  09
 pinMode(PCB_LED_D12,  OUTPUT);                                                               // LED on PCB board  10
 pinMode(LEDClockPin,  OUTPUT);                                                               // HC595 8-Bit Shift Registers
 pinMode(LEDDataPin,   OUTPUT);                                                               // HC595 8-Bit Shift Registers
 pinMode(LEDStrobePin, OUTPUT);                                                               // HC595 8-Bit Shift Registers
 SetStatusLED(0,0,1);                                                                         // Set the status LED to red
 Wire.begin();
 Previous_LDR_read = ReadLDR();                                                               // Set the initial LDR reading
 int32_t Tick = millis();  
 while (!Serial)  
 {if ((millis() - Tick) >5000) break;}  Tekstprintln("Serial started");                       // Wait max 5 seconds until serial port is started   
 SetStatusLED(0,1,0);
 InitDS3231Mod();                       Tekstprintln("DS3231 RTC software started");          // Start the DS3231 RTC-module even if not installed. It can be turned it on later in the menu
SetStatusLED(0,0,1);
 InitStorage();                         Tekstprintln("Stored settings loaded");               // Load settings from storage and check validity 
 StartLeds();                           Tekstprintln("LED strip started");                    // LED RainbowCycle
 if(Mem.BLEOn) { StartBLEService();     Tekstprintln("BLE started"); }                        // Start BLE service
SetStatusLED(1,0,0);
 if(Mem.WIFIOn){ StartWIFI_NTP();       Tekstprintln("WIFI started");   }                     // Start WIFI and optional NTP if Mem.WIFIOn = 1 
//SetStatusLED(0,1,0);
 if(Mem.DCF77On){StartDCF77();          Tekstprintln("DCF77 started");   }                    // Start DCF77
//SetStatusLED(0,0,0);
 SWversion();                                                                                 // Print the menu + version 
 GetTijd(1);                                                                                  // Get the time and print it
 Displaytime();                                                                               // Print the tekst time in the display 
 Tekstprintln(""); 
 msTick = millis();                                                                           // start the seconds loop counter
} 

//--------------------------------------------                                                //
// ARDUINO Loop
//--------------------------------------------
void loop() 
{
 Loopcounter++;
 if (Demo)          Demomode();
 else               EverySecondCheck();                                                        // Let the second led tick and run the clock program                                                                        // For Over The Air updates This loop block is necessary for ElegantOTA to handle reboot after OTA update.
CheckDevices();
}
//--------------------------------------------                                                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();                                                                                  // Something with BLE to do?
 NTPsyncEvent();                                                                              // Check for NTP events
 SerialCheck();                                                                               // Check serial port every second 
 ElegantOTA.loop();                                                                           // For Over The Air updates This loop block is necessary for ElegantOTA to handle reboot after OTA update.
 
                                  #ifdef DCF77MOD
 if(UseDCF)  DCF77Check();
                                  #endif  //DCF77MOD                               
                                  #ifdef ROTARYMOD      
 RotaryEncoderCheck(); 
                                  #endif  //ROTARYMOD
                                  #ifdef ONEWIREKEYPAD3x4   
 OnewireKeypad3x4Check(); 
                                  #endif  //ONEWIREKEYPAD3x4
                                  #ifdef ONEWIREKEYPAD3x1   
 OnewireKeypad3x1Check(); 
                                  #endif  //ONEWIREKEYPAD3x1
}
//--------------------------------------------                                                //
// COMMON Update routine done every second
//--------------------------------------------
void EverySecondCheck(void)
{
 static bool Toggle = true;
 uint32_t msLeap = millis() - msTick;                                                         // 
 if (msLeap >999)                                                                             // Every second enter the loop
 {
  msTick = millis();
  GetTijd(false);                                                                             // Get the time for the seconds 
  Toggle = !Toggle;                                                                           // Used to turn On or Off Leds
  UpdateStatusLEDs(Toggle);
                       #ifdef RGBCLOCK
  static uint8_t HC595value = 0;                     
  HC595bank(HC595value);                                                                      // Option to run
  if(++HC595value >= 24) HC595value = 0;                                                      // Let the HC595 shiftregister shift on one pin of the 24 
  SetSecondColour();                                                                          // Set the colour per second of 'IS' and 'WAS' 
                       #endif
  DimLeds(TestLDR);                                                                           // Every second an intensitiy check and update from LDR reading 
  if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();                                     // Enter the every minute routine after one minute; 
  Loopcounter=0;
 }  
}

//--------------------------------------------                                                //
// COMMON Update routine done every minute
//-------------------------------------------- 
void EveryMinuteUpdate(void)
{   
 lastminute = timeinfo.tm_min;  
 GetTijd(false);
 Displaytime();                                                                               //PrintRTCTime();
 DimLeds(true);   
 if(timeinfo.tm_hour != lasthour) EveryHourUpdate();
}
//--------------------------------------------                                                //
// COMMON Update routine done every hour
//--------------------------------------------
void EveryHourUpdate(void)
{
 if(Mem.WIFIOn)
   {
    if( WiFi.localIP()[0]==0 && strlen(Mem.Password)>4)                                      // If lost connection and a password entered
      {
       sprintf(sptext, "Disconnected from station, attempting reconnection.");
       Tekstprintln(sptext);
       WiFi.reconnect();
      }
   }
 lasthour = timeinfo.tm_hour;
 //if (!Mem.StatusLEDOn) SetStatusLED(0,0,0);                                                // If for some reason the LEDs are ON and after a MCU restart turn them off.  
 if( (timeinfo.tm_hour == Mem.TurnOffLEDsAtHH) && (Mem.TurnOffLEDsAtHH != Mem.TurnOnLEDsAtHH))
       { LEDsAreOff = true;  ClearScreen(); }                                                 // Is it time to turn off the LEDs?
 if(timeinfo.tm_hour == Mem.TurnOnLEDsAtHH) { LEDsAreOff = false;  Displaytime();  }                            // 
 Mem.NVRAMmem[lasthour] =(byte)((SumLDRreadshour / NoofLDRreadshour?NoofLDRreadshour:1));     // Update the average LDR readings per hour
 SumLDRreadshour  = 0;
 NoofLDRreadshour = 0;
 if (timeinfo.tm_mday != lastday) EveryDayUpdate();  
}
//--------------------------------------------                                                //
// COMMON Update routine done every day
//--------------------------------------------
void EveryDayUpdate(void)
{
 if(timeinfo.tm_mday != lastday) 
   {
    lastday           = timeinfo.tm_mday; 
    Previous_LDR_read = ReadLDR();                                                            // to have a start value
    MinPhotocell      = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
    MaxPhotocell      = Previous_LDR_read;                                                    // Stores maximum reading of photocell;
    Mem.ReconnectWIFI = 0;                                                                    // Reset WIFI reconnection counter     
//    StoreStructInFlashMemory();                                                             // 
    }
}

//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//-------------------------------------------- 
void UpdateStatusLEDs(bool Toggle)
{

 if(Mem.StatusLEDOn)   
   {
    SetStatusLED(Toggle && !WIFIConnected, Toggle && WIFIConnected, Toggle && deviceConnected); 
    digitalWrite(PCB_LED_D10, Toggle);                                                        //
    digitalWrite(PCB_LED_D12, Toggle);                                                        //
    digitalWrite(secondsPin,  !Toggle);     
   }
   else
   {
    SetStatusLED(0,0,0); 
    digitalWrite(PCB_LED_D10, 0);                                                             //
    digitalWrite(PCB_LED_D12, 0);                                                             //
    digitalWrite(secondsPin,  0);                                                             // on Nano ESP32 Turn the LED off    
   }
}
//--------------------------------------------                                                //
// COMMON check for serial input
//--------------------------------------------
void SerialCheck(void)
{
 String SerialString =""; 
 while (Serial.available())
    { 
     char c = Serial.read();                                                                  // Serial.write(c);
     if (c>31 && c<128) SerialString += c;                                                    // Allow input from Space - Del
     else c = 0;                                                                              // Delete a CR
    }
 if (SerialString.length()>0) 
    {
     ReworkInputString(SerialString);                                                         // Rework ReworkInputString();
     SerialString = "";
    }
}

//--------------------------------------------                                                //
// COMMON Reset to default settings
//--------------------------------------------
void Reset(void)
{
 Mem.Checksum         = 25065;                                                                //
 Mem.DisplayChoice    = DEFAULTCOLOUR;                                                        // Default colour scheme 
 Mem.OwnColour        = green;                                                                // Own designed colour.
 Mem.DimmedLetter     = dgray;
 Mem.BackGround       = black; 
 Mem.LanguageChoice   = 0;                                                                    // 0 = NL, 1 = UK, 2 = DE, 3 = FR, 4 = Wheel
 Mem.LightReducer     = SLOPEBRIGHTNESS;                                                      // Factor to dim ledintensity with. Between 0.1 and 1 in steps of 0.05
 Mem.UpperBrightness  = MAXBRIGHTNESS;                                                        // Upper limit of Brightness in bits ( 1 - 1023)
 Mem.LowerBrightness  = LOWBRIGHTNESS;                                                        // Lower limit of Brightness in bits ( 0 - 255)
 Mem.TurnOffLEDsAtHH  = 0;                                                                    // Display Off at nn hour
 Mem.TurnOnLEDsAtHH   = 0;                                                                    // Display On at nn hour Not Used
 Mem.BLEOn            = 1;                                                                    // Default BLE On
 Mem.UseBLELongString = 0;                                                                    // Default off. works only with iPhone/iPad with BLEserial app
 Mem.NTPOn            = 0;                                                                    // NTP default off
 Mem.WIFIOn           = 0;                                                                    // WIFI default off
 Mem.ReconnectWIFI    = 0;                                                                    // Correct time if necessary in seconds
 Mem.WIFINoOfRestarts = 0;                                                                    //  
 Mem.DCF77On          = 0;                                                                    // Default off
 Mem.UseDS3231        = 0;                                                                    // Default off
 //Mem.LEDstrip       = 0;   // This definition should not be reset                           // 0 = SK6812, 1=WS2812, 2=White LEDs
 Previous_LDR_read    = ReadLDR();                                                            // Read LDR to have a start value. max = 4096/8 = 255
 MinPhotocell         = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
 MaxPhotocell         = Previous_LDR_read;                                                    // Stores maximum reading of photocell;                                            
 TestLDR              = 0;                                                                    // If true LDR display is printed every second
 
 strcpy(Mem.Ssid,"");                                                                         // Default SSID
 strcpy(Mem.Password,"");                                                                     // Default password
 strcpy(Mem.BLEbroadcastName,"NanoESP32Clock");
 strcpy(Mem.Timezone,"CET-1CEST,M3.5.0,M10.5.0/3");                                           // Central Europe, Amsterdam, Berlin etc.                                                         // WIFI On  

 Tekstprintln("**** Reset of preferences ****"); 
 StoreStructInFlashMemory();                                                                  // Update Mem struct       
 GetTijd(false);                                                                                  // Get the time and store it in the proper variables
 SWversion();                                                                                 // Display the version number of the software
 Displaytime();
}
//--------------------------------------------                                                //
// COMMON common print routines
//--------------------------------------------
void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst);  SendMessageBLE(tekst);sptext[0]=0;   } 
void Tekstprintln(char const *tekst)  { sprintf(sptext,"%s\n",tekst); Tekstprint(sptext);  }
void TekstSprint(char const *tekst)   { printf(tekst); sptext[0]=0;}                          // printing for Debugging purposes in serial monitor 
void TekstSprintln(char const *tekst) { sprintf(sptext,"%s\n",tekst); TekstSprint(sptext); }
//--------------------------------------------                                                //
//  COMMON String upper
//--------------------------------------------
void to_upper(char* string)
{
 const char OFFSET = 'a' - 'A';
 while (*string) {(*string >= 'a' && *string <= 'z') ? *string -= OFFSET : *string;   string++;  }
}
//--------------------------------------------                                                //
// COMMON Constrain a string with integers
// The value between the first and last character in a string is returned between the low and up bounderies
//--------------------------------------------
int SConstrainInt(String s,byte first,byte last,int low,int up){return constrain(s.substring(first, last).toInt(), low, up);}
int SConstrainInt(String s,byte first,          int low,int up){return constrain(s.substring(first).toInt(), low, up);}
//--------------------------------------------                                                //
// COMMON Init and check contents of EEPROM
//--------------------------------------------
void InitStorage(void)
{
 // if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){ Tekstprintln("Card Mount Failed");   return;}
 // else Tekstprintln("SPIFFS mounted"); 

 GetStructFromFlashMemory();
 if( Mem.Checksum != 25065)
   {
    sprintf(sptext,"Checksum (25065) invalid: %d\n Resetting to default values",Mem.Checksum); 
    Tekstprintln(sptext); 
    Reset();                                                                                  // If the checksum is NOK the Settings were not set
   }
 Mem.LanguageChoice  = _min(Mem.LanguageChoice, 4);                                           // Constrain the value to valid ranges 
 Mem.DisplayChoice   = _min(Mem.DisplayChoice, ANALOOG);                                      // Constrain the value to valid ranges 
 if(Mem.OwnColour == 0) Mem.OwnColour = green;                                                // If memory is empty cq black colour then store default value, green  
 Mem.LightReducer    = constrain(Mem.LightReducer,1,250);                                     // 
 Mem.LowerBrightness = constrain(Mem.LowerBrightness, 1, 250);                                // 
 Mem.UpperBrightness = _min(Mem.UpperBrightness, 255); 
 if(strlen(Mem.Password)<5 || strlen(Mem.Ssid)<3)     Mem.WIFIOn = Mem.NTPOn = 0;             // If ssid or password invalid turn WIFI/NTP off
 if(Mem.LEDstrip  > 1) Mem.LEDstrip = 0;                                                      // Default SK6812
 StoreStructInFlashMemory();
}
//--------------------------------------------                                                //
// COMMON Store mem.struct in FlashStorage or SD
// Preferences.h  
//--------------------------------------------
void StoreStructInFlashMemory(void)
{
  FLASHSTOR.begin("Mem",false);  
  FLASHSTOR.putBytes("Mem", &Mem , sizeof(Mem) );
  FLASHSTOR.end();          
  
// Can be used as alternative
//  SPIFFS
//  File myFile = SPIFFS.open("/MemStore.txt", FILE_WRITE);
//  myFile.write((byte *)&Mem, sizeof(Mem));
//  myFile.close();
 }
//--------------------------------------------                                                //
// COMMON Get data from FlashStorage
// Preferences.h
//--------------------------------------------
void GetStructFromFlashMemory(void)
{
 FLASHSTOR.begin("Mem", false);
 FLASHSTOR.getBytes("Mem", &Mem, sizeof(Mem) );
 FLASHSTOR.end(); 

// Can be used as alternative if no SD card
//  File myFile = SPIFFS.open("/MemStore.txt");  FILE_WRITE); myFile.read((byte *)&Mem, sizeof(Mem));  myFile.close();

 sprintf(sptext,"Mem.Checksum = %d",Mem.Checksum);Tekstprintln(sptext); 
}

//--------------------------------------------                                                //
// COMMON Version info
//--------------------------------------------
void SWversion(void) 
{ 
 #define FILENAAM (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
 PrintLine(35);
 for (uint8_t i = 0; i < sizeof(menu) / sizeof(menu[0]); Tekstprintln(menu[i++]));
 PrintLine(35);
 sprintf(sptext,"Display off between: %02dh - %02dh",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);  Tekstprintln(sptext);
                        #ifdef RGBCLOCK
 byte dp = Mem.DisplayChoice;                        
 sprintf(sptext,"Display choice: %s",dp==0?"Yellow":dp==1?"Hourly":dp==2?"White":
                      dp==3?"All Own":dp==4?"Own":dp==5?"Wheel":dp==6?"Digital":dp==7?"Analog":"NOP");      
                                                                                              Tekstprintln(sptext);
                        #endif //RGBCLOCK                                                                                             
 sprintf(sptext,"Slope: %d     Min: %d     Max: %d ",Mem.LightReducer, Mem.LowerBrightness,Mem.UpperBrightness);  
                                                                                             Tekstprintln(sptext);
 sprintf(sptext,"SSID: %s", Mem.Ssid);                                                       Tekstprintln(sptext); 
// sprintf(sptext,"Password: %s", Mem.Password);                                             Tekstprintln(sptext);
 sprintf(sptext,"BLE name: %s", Mem.BLEbroadcastName);                                       Tekstprintln(sptext);
 sprintf(sptext,"IP-address: %d.%d.%d.%d/update", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );  Tekstprintln(sptext);
 sprintf(sptext,"Timezone:%s", Mem.Timezone);                                                Tekstprintln(sptext); 
 sprintf(sptext,"%s %s %s %s", Mem.WIFIOn?"WIFI=On":"WIFI=Off", 
                               Mem.NTPOn? "NTP=On":"NTP=Off",
                               Mem.BLEOn? "BLE=On":"BLE=Off",
                               Mem.UseBLELongString? "FastBLE=On":"FastBLE=Off" );           Tekstprintln(sptext);
 //sprintf(sptext,"Language choice: %s",ch==0?"NL":ch==1?"UK":ch==2?"DE":ch==3?"FR":ch==4?"Rotate language":"NOP");         Tekstprintln(sptext);
 sprintf(sptext,"LED strip(%%): %s | %s DS3231", Mem.LEDstrip==0?"SK6812":Mem.LEDstrip==1?"WS2812":"NOP",
         !DS3231Installed?"Not installed":Mem.UseDS3231==0?"NOT using":Mem.UseDS3231==1? "Using":"NOP");   
                                                                                             Tekstprintln(sptext);        
 sprintf(sptext,"Software: %s",FILENAAM);                                                    Tekstprintln(sptext);  //VERSION); 
 PrintRTCTime(); Tekstprintln(""); 
 PrintLine(35);
}
//--------------------------------------------                                                //
// COMMON PrintLine
//--------------------------------------------
void PrintLine(byte Lengte)
{
 for(int n=0; n<Lengte; n++) sptext[n]='_';
 sptext[Lengte] = 0;
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
//  COMMON Input from Bluetooth or Serial
//--------------------------------------------
void ReworkInputString(String InputString)
{
 if(InputString.length()> 40){Serial.printf("Input string too long (max40)\n"); return;}      // If garbage return
 for (int n=0; n<InputString.length()+1; n++)                                                 // remove CR and LF
    if (InputString[n] == 10 || InputString[n]==13) InputString.remove(n,1);
   
 sptext[0] = 0;                                                                               // Empty the sptext string
 
 if(InputString[0] > 31 && InputString[0] <127)                                               // Does the string start with a letter?
  { 
  switch (InputString[0])
   {
    case 'A':
    case 'a': 
            if (InputString.length() >4 )
            {
             InputString.substring(1).toCharArray(Mem.Ssid,InputString.length());
             sprintf(sptext,"SSID set: %s", Mem.Ssid);  
            }
            else sprintf(sptext,"**** Length fault. Use between 4 and 30 characters ****");
            break;
    case 'B':
    case 'b': 
           if (InputString.length() >4 && InputString.length() <40)
            {  
             InputString.substring(1).toCharArray(Mem.Password,InputString.length());
             sprintf(sptext,"Password set: %s\n Enter @ to reset ESP32 and connect to WIFI and NTP\n WIFI and NTP are turned ON", Mem.Password); 
             Mem.NTPOn        = 1;                                                         // NTP On
             Mem.WIFIOn       = 1;                                                         // WIFI On  
            }
            else sprintf(sptext,"%s,**** Length fault. Use between 4 and 39 characters ****",Mem.Password);
            break;   
    case 'C':
    case 'c': 
           if (InputString.length() >4 &&InputString.length() <30 )
            {  
             InputString.substring(1).toCharArray(Mem.BLEbroadcastName,InputString.length());
             sprintf(sptext,"BLE broadcast name set: %s", Mem.BLEbroadcastName); 
             Mem.BLEOn = 1;                                                                   // BLE On
            }
            else sprintf(sptext,"**** Length fault. Use between 4 and 29 characters ****");
            break;      
    case 'D':
    case 'd':  
            if (InputString.length() == 9 )
              {
               timeinfo.tm_mday = (int) SConstrainInt(InputString,1,3,0,31);
               timeinfo.tm_mon  = (int) SConstrainInt(InputString,3,5,0,12) - 1; 
               timeinfo.tm_year = (int) SConstrainInt(InputString,5,9,2000,9999) - 1900;
               if (DS3231Installed)
                 {
                  sprintf(sptext,"Time set in external RTC module");  
                  SetDS3231Time();
                  PrintDS3231Time();
                 }
               else sprintf(sptext,"No external RTC module detected");
               }            
            else sprintf(sptext,"****\nLength fault. Enter Dddmmyyyy\n****");
            break;
    case 'E':
    case 'e':  
           if (InputString.length() >2 )
            {  
             InputString.substring(1).toCharArray(Mem.Timezone,InputString.length());
             sprintf(sptext,"Timezone set: %s", Mem.Timezone); 
            }
            else sprintf(sptext,"**** Length fault. Use more than 2 characters ****");
            break;  
    case 'F':
    case 'f':  
             if (InputString.length() == 9 )
               {
                LetterColor = Mem.OwnColour = HexToDec(InputString.substring(1,9));           // Display letter color 
                sprintf(sptext,"Font colour stored: 0X%08X", Mem.OwnColour);
                Tekstprintln("**** Own colour changed ****");    
                LedsOff(); 
                Displaytime();
               }
             else sprintf(sptext,"****Length fault. Enter Frrggbb hexadecimal (0 - F)****\nStored: 0X%08X", Mem.OwnColour);              
             break;
    case 'G':
    case 'g':  
             if (InputString.length() == 7 )
               {
                Mem.DimmedLetter = HexToDec(InputString.substring(1,7));                      // Display letter color 
                sprintf(sptext,"Dimmed colour stored; 0X%08X", Mem.DimmedLetter);
                Tekstprintln("**** Dimmed font colour changed ****");    
                Displaytime();
               }
             else sprintf(sptext,"****Length fault. Enter Grrggbb hexadecimal (0 - F)****\nStored: 0X%08X", Mem.DimmedLetter);            
             break;
    case 'H':
    case 'h':  
             if (InputString.length() == 7 )
               {
                Mem.BackGround = HexToDec(InputString.substring(1,7));                        // Display character color 
                sprintf(sptext,"Own colour stored: 0X%08X", Mem.BackGround);
                Tekstprintln("**** BackGround colour changed ****");    
                Displaytime();
               }
             else sprintf(sptext,"****Length fault. Enter Hrrggbb hexadecimal (0 - F)****\nStored: 0X%08X", Mem.BackGround);           
             break;            
    case 'I':
    case 'i': 
            SWversion();
            break;
    case 'J':
    case 'j':
             sprintf(sptext,"**** Length fault. Enter J or JJ ****");
             if (InputString.length() == 1)
               {   
                Mem.UseDS3231 = 1 - Mem.UseDS3231; 
                sprintf(sptext,"Use DS3231 is %s", Mem.UseDS3231?"ON":"OFF" );
               } 
             if (InputString.length() == 2 && InputString[1] =='J')  // JJ must be entered
              {
               UseDCF = 1 - UseDCF;
               sprintf(sptext,"Use DCF-receiver: %s",UseDCF ? "Yes" : "No");
              }                                 
             break; 
    case 'K':
    case 'k':
             TestLDR = 1 - TestLDR;                                                           // If TestLDR = 1 LDR reading is printed every second instead every 30s
             sprintf(sptext,"TestLDR: %s \nLDR reading, min and max of one day, %%Out, loops per second and time",TestLDR? "On" : "Off");
             break;      
    case 'L':                                                                                 // Language to choose
    case 'l':
   
    case 'M':
    case 'm':
             sprintf(sptext,"**** Length fault M. ****");
             if (InputString.length() == 1)
               {   
                Demo = false; 
                sprintf(sptext,"Demo mode: %s",Demo?"ON":"OFF"); 
   
               }
             if (InputString.length() >1 && InputString.length() < 6 )
              {
                MilliSecondValue = InputString.substring(1,5).toInt();                
                Demo = true;                                                                 // Toggle Demo mode
                sprintf(sptext,"Demo mode: %s MillisecondTime=%d",Demo?"ON":"OFF", MilliSecondValue); 
              }
             break; 
    case 'N':
    case 'n':
             sprintf(sptext,"**** Length fault N. ****");    
             if (InputString.length() == 1 )         Mem.TurnOffLEDsAtHH = Mem.TurnOnLEDsAtHH = 0;
             if (InputString.length() == 5 )
              {
               Mem.TurnOffLEDsAtHH =(byte) InputString.substring(1,3).toInt(); 
               Mem.TurnOnLEDsAtHH  =(byte) InputString.substring(3,5).toInt(); 
              }
             Mem.TurnOffLEDsAtHH = _min(Mem.TurnOffLEDsAtHH, 23);
             Mem.TurnOnLEDsAtHH  = _min(Mem.TurnOnLEDsAtHH, 23); 
             sprintf(sptext,"Display is OFF between %2d:00 and %2d:00", Mem.TurnOffLEDsAtHH,Mem.TurnOnLEDsAtHH );
             break;
    case 'O':
    case 'o':
             sprintf(sptext,"**** Length fault O. ****");
             if(InputString.length() == 1)
               {
                LEDsAreOff = !LEDsAreOff;
                sprintf(sptext,"Display is %s", LEDsAreOff?"OFF":"ON" );
                if(LEDsAreOff) { ClearScreen();}                                              // Turn the display off
                else {
                  Tekstprintln(sptext); 
                  lastminute = 99;
 //                 Displaytime();                                                              // Turn the display on   
                }
               }
             break;                                                                   
    case 'P':
    case 'p':  
             sprintf(sptext,"**** Length fault P. ****");  
             if(InputString.length() == 1)
               {
                Mem.StatusLEDOn = !Mem.StatusLEDOn;
                UpdateStatusLEDs(0);
                sprintf(sptext,"StatusLEDs are %s", Mem.StatusLEDOn?"ON":"OFF" );            // UpdateStatusLEDs(Toggle); in EverySecond() will turn On or Off the LEDs    
               }
             break;        
                       #ifdef RGBCLOCK
    case 'q':
    case 'Q':   
             sprintf(sptext,"**** Length fault Q. ****");   
             if (InputString.length() == 1 )
               {
             Tekstprintln("  Q0= Default colour");
             Tekstprintln("  Q1= Hourly colour");
             Tekstprintln("  Q2= All white");
             Tekstprintln("  Q3= All Own colour");
             Tekstprintln("  Q4= Own colour, HETISWAS changing");
             Tekstprint(  "  Q5= Wheel colour");
             Tekstprint(  "  Q6= Digital display");
             sptext[0]=0;
               }
             if (InputString.length() == 2 )
               {
                Mem.DisplayChoice = (byte) InputString.substring(1,2).toInt(); 
                if (Mem.DisplayChoice>6) Mem.DisplayChoice = 0;
                sprintf(sptext,"Display choice: Q%d", Mem.DisplayChoice);
                lastminute = 99;                                                              // Force a minute update
               }    
             //Displaytime();   
             break;
                       #endif //RGBCLOCK             
    case 'R':
    case 'r':
             sprintf(sptext,"**** Length fault R. ****");       
             if (InputString.length() == 1)
               {   
                Reset();
                sprintf(sptext,"\nReset to default values: Done");
                Displaytime();                                                                // Turn on the display with proper time
               }                                
             break;      
    case 'S':                                                                                 // Slope. factor ( 0 - 1) to multiply brighness (0 - 255) with 
    case 's':
             sprintf(sptext,"**** Length fault S. ****");    
            if (InputString.length() > 1 && InputString.length() < 5)
               {                
                Mem.LightReducer = (byte) SConstrainInt(InputString,1,1,255);
                sprintf(sptext,"Slope brightness changed to: %d%%",Mem.LightReducer);
               }
             else sprintf(sptext,"**** Input fault. \nEnter Snnn where n between 1 and 255");
              break;                    
    case 'T':
    case 't':
//                                                                                            //
             if(InputString.length() == 7)  // T125500
               {
                timeinfo.tm_hour = (int) SConstrainInt(InputString,1,3,0,23);
                timeinfo.tm_min  = (int) SConstrainInt(InputString,3,5,0,59); 
                timeinfo.tm_sec  = (int) SConstrainInt(InputString,5,7,0,59);
               if (DS3231Installed)
                 {
                  sprintf(sptext,"Time set in external RTC module");  
                  SetDS3231Time();
                  PrintDS3231Time();
                 }
               else sprintf(sptext,"No external RTC module detected");
               } 
             else sprintf(sptext,"**** Length fault. Enter Thhmmss ****");
             break;            
    case 'U':                                                                                 // factor to multiply brighness (0 - 255) with 
    case 'u':
            if (InputString.length() > 1 &&  InputString.length() < 5)
               {    
                Mem.UpperBrightness = SConstrainInt(InputString,1,1,255);
                sprintf(sptext,"Upper brightness changed to: %d bits",Mem.UpperBrightness);
               }
            else sprintf(sptext,"**** Input fault. \nEnter Unnn where n between 1 and 255");
 
              break;  
    case 'V':
    case 'v':  
             if (InputString.length() > 1 &&  InputString.length() < 5)
               {      
                Mem.LowerBrightness = (byte) SConstrainInt(InputString,1,0,255);
                sprintf(sptext,"Lower brightness: %d bits",Mem.LowerBrightness);
               }
             else sprintf(sptext,"**** Input fault. \nEnter Vnnn where n between 1 and 255");               
             break;      
    case 'W':
    case 'w':
             if (InputString.length() == 1)
               {   
                Mem.WIFIOn = 1 - Mem.WIFIOn; 
                Mem.ReconnectWIFI = 0;                                                        // Reset WIFI reconnection counter 
                Mem.NTPOn = Mem.WIFIOn;                                                       // If WIFI is off turn NTP also off
                sprintf(sptext,"WIFI is %s after restart", Mem.WIFIOn?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter W ****");
             break; 
    case 'X':
    case 'x':
             if (InputString.length() == 1)
               {   
                Mem.NTPOn = 1 - Mem.NTPOn; 
                sprintf(sptext,"NTP is %s after restart", Mem.NTPOn?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter X ****");
             break; 
    case 'Y':
    case 'y':
             if (InputString.length() == 1)
               {   
                Mem.BLEOn = 1 - Mem.BLEOn; 
                sprintf(sptext,"BLE is %s after restart", Mem.BLEOn?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter Y ****");
             break; 
    case 'Z':
    case 'z':
             if (InputString.length() == 1)
               {   
                Mem.UseBLELongString = 1 - Mem.UseBLELongString; 
                sprintf(sptext,"Fast BLE is %s", Mem.UseBLELongString?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter U ****");
             break; 
//--------------------------------------------                                                //        
     case '!':                                                                                // Print the NTP, RTC and DS3231 time
             if (InputString.length() == 1)  PrintAllClockTimes();
             break;       
    case '@':
             if (InputString.length() == 1)
               {   
                Tekstprintln("\n*********\n ESP restarting\n*********\n");
                ESP.restart();   
               }                                
             else sprintf(sptext,"**** Length fault. Enter @ ****");
             break;     
    case '#':
             if (InputString.length() == 1)
               {
                Zelftest = 1 - Zelftest; 
                sprintf(sptext,"Zelftest: %s", Zelftest?"ON":"OFF" ); 
                Selftest();   
                Displaytime();                                                                // Turn on the display with proper time
               }                                
             else sprintf(sptext,"**** Length fault. Enter # ****");
             break; 
    case '$':
             if (InputString.length() == 1)
               {   
                Mem.DCF77On = 1 - Mem.DCF77On; 
                sprintf(sptext,"DCF77 is %s after restart", Mem.DCF77On?"used":"NOT used" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter $ ****");
             break; 
     case '%':
             if (InputString.length() == 1)
               {   
                Mem.LEDstrip = 1 - Mem.LEDstrip; 
                sprintf(sptext,"LED strip is %s after restart", Mem.LEDstrip?"WS2812":"SK6812" );
               }                                
             else sprintf(sptext,"**** Length fault. ****");
             break; 
//                                                                                            //             
    case '&':
             sprintf(sptext,"**** Length fault &. ****");                                     // Forced get NTP time and update the DS32RTC module
             if (InputString.length() == 1)
              {
               GetNTPtime(false);                                                                 // Force a NTP time update  
               SetDS3231Time();                                                               // Set the time of the external RTC module  
               SetRTCTime();                                                                  // Set the time in the internal Arduino RTC
               PrintAllClockTimes();
               } 
             break;                                                                           // Toggle DCF Signal on Display
    case '*':
            if (InputString.length() == 1)
              {
               SeeDCFsignalInDisplay = 1 - SeeDCFsignalInDisplay;
               sprintf(sptext,"SeeDCFsignal: %s",SeeDCFsignalInDisplay ? "Yes" : "No ");
               }
             break;                      
    case '0':
    case '1':
    case '2':        
             if (InputString.length() == 6)                                                    // For compatibility input with only the time digits
              {
               timeinfo.tm_hour = (int) SConstrainInt(InputString,0,2,0,23);
               timeinfo.tm_min  = (int) SConstrainInt(InputString,2,4,0,59); 
               timeinfo.tm_sec  = (int) SConstrainInt(InputString,4,6,0,59);
               if (DS3231Installed)
                 {
                  sprintf(sptext,"Time set in external RTC module");  
                  SetDS3231Time();
                  PrintDS3231Time();
                 }
               else sprintf(sptext,"No external RTC module detected");
               } 
    default: break;
    }
  }  
 Tekstprintln(sptext); 
 StoreStructInFlashMemory();                                                                                                          // Update EEPROM                                     
 InputString = "";
}


//--------------------------------------------                                                //
// LDR reading are between 0 and 255. 
// ESP32 analogue read is between 0 - 4096 --   is: 4096 / 8
//--------------------------------------------
int ReadLDR(void)
{
 return analogRead(PhotoCellPin)/16;
}
//--------------------------------------------
// LED CLOCK Control the LEDs on the Nano ESP32
// 0 Low is LED off. Therefore the value is inversed with the ! Not 
//--------------------------------------------
void SetStatusLED(bool Red, bool Green, bool Blue)
{
 digitalWrite(LED_RED, !Red);                                                                 // !Red (not Red) because 1 or HIGH is LED off
 digitalWrite(LED_GREEN, !Green);
 digitalWrite(LED_BLUE, !Blue);
}

//--------------------------------------------                                                //
// CLOCK set the LED's for displaying
//--------------------------------------------
void Displaytime(void)
{
 DisplayTime();                                                                               // Function in BW and RGBW header file
}

//--------------------------------------------                                                //
//  HC595 LED  load the shiftbits in the LED display buffer
//--------------------------------------------
void ShiftInTime(const char *Tekst, byte num , byte Displaynr, byte Shiftbit, byte ShiftIn)
{  
 if (!NoTextInLeds && strlen(Tekst) > 0 )
     {sprintf(sptext,"%s ",Tekst); Tekstprint(sptext); }                                      // Print the text  
 switch (Displaynr)
  {
 case 1:
        Display1 |= Shiftbit << ShiftIn;
        break;
 case 2:
        Display2 |= Shiftbit << ShiftIn;
        break;
 case 3:
        Display3 |= Shiftbit << ShiftIn;
        break; 
 }     
}



// --------------------Colour Clock Light functions -----------------------------------

//--------------------------------------------                                                //
//  LED Set color for LEDs in strip and print tekst
//---------------------------------------------
void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor)
{ 
 Stripfill(RGBWColor, FirstLed, ++LastLed - FirstLed );                                        //
 if (!NoTextInLeds && strlen(Tekst) > 0 )
     {sprintf(sptext,"%s ",Tekst); Tekstprint(sptext); }                                      // Print the text  
}
//--------------------------------------------
//  LED Set color for one LED
//--------------------------------------------
void ColorLed(int Lednr, uint32_t RGBWColor)
{   
 Stripfill(RGBWColor, Lednr, 1 );
}
//--------------------------------------------                                                //
//  LED Clear display settings of the LED's
//--------------------------------------------
void LedsOff(void) 
{ 
 Stripfill(0, 0, NUM_LEDS );                                                                  // 
}
//--------------------------------------------                                                //
//  LED Push data in LED strip to commit the changes
//--------------------------------------------
void ShowLeds(void)
{
 LEDstrip.show();
}
//--------------------------------------------                                                //
//  LED Set brighness of LEDs
//--------------------------------------------
void SetBrightnessLeds(byte Bright)
{
 analogWrite(PWMpin, Bright);                                                                 // Set Brightness of HC595 white LED-strip    
 LEDstrip.setBrightness(Bright);                                                              // Set brightness of RGB(W) LEDs on Pin D5   
 ShowLeds();
}
//--------------------------------------------
//  LED Fill the strip array
//--------------------------------------------
void Stripfill(uint32_t RGBWColor, int FirstLed, int NoofLEDs)
{   
 LEDstrip.fill(RGBWColor, FirstLed, NoofLEDs);
}
//--------------------------------------------
//  LED Strip Get Pixel Color 
//--------------------------------------------
uint32_t StripGetPixelColor(int Lednr)
{
return(LEDstrip.getPixelColor(Lednr));
}
//--------------------------------------------                                                //
//  LED convert HSV to RGB  h is from 0-360, s,v values are 0-1
//  r,g,b values are 0-255
//--------------------------------------------
uint32_t HSVToRGB(double H, double S, double V) 
{
  int i;
  double r, g, b, f, p, q, t;
  if (S == 0)  {r = V;  g = V;  b = V; }
  else
  {
    H >= 360 ? H = 0 : H /= 60;
    i = (int) H;
    f = H - i;
    p = V * (1.0 -  S);
    q = V * (1.0 - (S * f));
    t = V * (1.0 - (S * (1.0 - f)));
    switch (i) 
    {
     case 0:  r = V;  g = t;  b = p;  break;
     case 1:  r = q;  g = V;  b = p;  break;
     case 2:  r = p;  g = V;  b = t;  break;
     case 3:  r = p;  g = q;  b = V;  break;
     case 4:  r = t;  g = p;  b = V;  break;
     default: r = V;  g = p;  b = q;  break;
    }
  }
return FuncCRGBW((int)(r*255), (int)(g*255), (int)(b*255), 0 );                                // R, G, B, W 
}
//--------------------------------------------                                                //
//  LED function to make RGBW color
//--------------------------------------------
uint32_t FuncCRGBW( uint32_t Red, uint32_t Green, uint32_t Blue, uint32_t White)
{ 
 return ( (White<<24) + (Red << 16) + (Green << 8) + Blue );
}
//--------------------------------------------                                                //
//  LED functions to extract RGBW colors
//--------------------------------------------
 uint8_t Cwhite(uint32_t c) { return (c >> 24);}
 uint8_t Cred(  uint32_t c) { return (c >> 16);}
 uint8_t Cgreen(uint32_t c) { return (c >> 8); }
 uint8_t Cblue( uint32_t c) { return (c);      }

//--------------------------------------------                                                //
// LED Dim the leds measured by the LDR and print values
// LDR reading are between 0 and 255. The Brightness send to the LEDs is between 0 and 255
//--------------------------------------------
void DimLeds(bool print) 
{ 
  int LDRread = ReadLDR();                                                                    // ESP32 analoge read is between 0 - 4096, reduce it to 0-1024                                                                                                   
  int LDRavgread = (4 * Previous_LDR_read + LDRread ) / 5;                                    // Read lightsensor and avoid rapid light intensity changes
  Previous_LDR_read = LDRavgread;                                                             // by using the previous reads
  OutPhotocell = (uint32_t)((Mem.LightReducer * sqrt(255*LDRavgread))/100);                   // Linear --> hyperbolic with sqrt. Result is between 0-255
  MinPhotocell = min(MinPhotocell, LDRavgread);                                               // Lowest LDR measurement
  MaxPhotocell = max(MaxPhotocell, LDRavgread);                                               // Highest LDR measurement
  OutPhotocell = constrain(OutPhotocell, Mem.LowerBrightness, Mem.UpperBrightness);           // Keep result between lower and upper boundery en calc percentage
  SumLDRreadshour += LDRavgread;    NoofLDRreadshour++;                                       // For statistics LDR readings per hour
  if(print)
  {
  // sprintf(sptext,"LDR:%3d Avg:%3d (%3d-%3d) Out:%3d=%2d%% Loop(%ld) ",
  //      LDRread,LDRavgread,MinPhotocell,MaxPhotocell,OutPhotocell,(int)(OutPhotocell/2.55),Loopcounter);    
 if (Mem.UseDS3231)   sprintf(sptext,"LDR:%3d=%2d%% %5lld l/s %0.0f°C ",
               LDRread,(int)(OutPhotocell*100/255),Loopcounter,GetDS3231Temp()); 
  else               
   sprintf(sptext,"LDR:%3d=%2d%% %5lld l/s ",
               LDRread,(int)(OutPhotocell*100/255),Loopcounter);   
   Tekstprint(sptext);
   PrintTimeHMS();  
  }
 if(LEDsAreOff) OutPhotocell = 0;
 SetBrightnessLeds(OutPhotocell);     // values between 0 and 255
}
//--------------------------------------------                                                //
//  LED Turn On en Off the LED's
//--------------------------------------------
void Play_Lights()
{
 for(int i=0; i<NUM_LEDS; i++) { ColorLeds("",i,i,chromeyellow); ShowLeds(); }
 WhiteOverRainbow(0, 0, 5 );
 WhiteOverRainbow(0,0, 5 );  // wait, whiteSpeed, whiteLength
 LedsOff();
}
//--------------------------------------------
//  LED Blink UUR
//--------------------------------------------
void BlinkUUR(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(); delay(Delayms); UUR; Laatzien(); delay(Delayms);} 
}
//--------------------------------------------
//  LED Blink HET IS WAS
//--------------------------------------------
void BlinkHETISWAS (int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(); delay(Delayms); HET; IS; WAS; Laatzien(); delay(Delayms);} 
}
//--------------------------------------------
//  LED Blink PRECIES
//--------------------------------------------
void BlinkTWAALF(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(); delay(Delayms); TWAALF; Laatzien(); delay(Delayms);} 
}

//--------------------------------------------                                                //
//  DISPLAY
//  Clear the display
//--------------------------------------------
void ClearScreen(void)
{
 LedsOff();
}
//--------------------------------------------                                                //
//  LED Wheel
//  Input a value 0 to 255 to get a color value.
//  The colours are a transition r - g - b - back to r.
//--------------------------------------------
uint32_t Wheel(byte WheelPos) 
{
 WheelPos = 255 - WheelPos;
 if(WheelPos < 85)   { return FuncCRGBW( 255 - WheelPos * 3, 0, WheelPos * 3, 0);  }
 if(WheelPos < 170)  { WheelPos -= 85;  return FuncCRGBW( 0,  WheelPos * 3, 255 - WheelPos * 3, 0); }
 WheelPos -= 170;      
 return FuncCRGBW(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

//--------------------------------------------                                                //
//  LED RainbowCycle
//--------------------------------------------
void StartLeds(void) 
{
 switch (Mem.LEDstrip)
    {
      case 0: LEDstrip = LED6812strip; 
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;                                                            // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000; 
              sprintf(sptext,"LED strip is SK6812");
              LEDstrip.setBrightness(BRIGHTNESS);                                                          // Set initial brightness of LEDs   
             break;
      case 1: LEDstrip = LED2812strip; 
              white  = 0xFFFFFF;
              lgray  = 0x666666;                                                              // R, G and B on together gives white light
              gray   = 0x333333;
              dgray  = 0x222222;  
              sprintf(sptext,"LED strip is WS2812");
              LEDstrip.setBrightness(BRIGHTNESS);                                                          // Set initial brightness of LEDs          
              break;
      case 2: 
              sprintf(sptext,"LED strip is White LEDs");

              break;
     default: LEDstrip = LED6812strip;
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;                                                            // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000;      
              sprintf(sptext,"LED strip is SK6812");
              LEDstrip.setBrightness(BRIGHTNESS);                                                          // Set initial brightness of LEDs 
    }
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
//  LED RainbowCycle
//--------------------------------------------
void RainbowCycle(uint8_t wait) 
{
 uint16_t i, j;
 for(j=0; j<256 * 5; j++) 
   {                                                                                          // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) ColorLeds("",i,i,Wheel(((i * 256 / NUM_LEDS) + j) & 255));
    ShowLeds();
    delay(wait);
  }
}
//--------------------------------------------                                                //
//  LED WhiteOverRainbow
//--------------------------------------------
void WhiteOverRainbow(uint32_t wait, uint8_t whiteSpeed, uint32_t whiteLength ) 
{
 if(whiteLength >= NUM_LEDS) whiteLength = NUM_LEDS - 1;
 uint32_t head = whiteLength - 1;
 uint32_t tail = 0;
 uint32_t loops = 1;
 uint32_t loopNum = 0;
 static unsigned long lastTime = 0;
 while(true)
  {
    for(uint32_t j=0; j<256; j++) 
     {
      for(uint32_t i=0; i<=NUM_LEDS; i++) 
       {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) )
              ColorLeds("",i,i,0XFFFFFF );
        else  
              ColorLeds("",i,i,Wheel(((i * 256 / NUM_LEDS) + j) & 255));
       }
      if(millis() - lastTime > whiteSpeed) 
       {
        head++;        tail++;
        if(head == NUM_LEDS) loopNum++;
        lastTime = millis();
      }
      if(loopNum == loops) return;
      head %= NUM_LEDS;
      tail %= NUM_LEDS;
      ShowLeds();
      delay(wait);
    }
  }  // end while
}
//--------------------------------------------                                                //
//  LED Place digits 0 - 9 in Matrix display
// First row and column = 0, PosX,PosY is left top position of 3x5 digit
// Calculate position LED #define MATRIX_WIDTH 12 #define MATRIX_HEIGHT 12
//--------------------------------------------
void Zet_Pixel(byte Cijfer,byte Pos_X, byte Pos_Y) 
{ 
 uint32_t LEDnum;
 uint32_t OrgColor;
 for(int i=0;i<3;i++)
  {  
   for(int j=0;j<5;j++)
   {
    int c = pgm_read_byte_near ( &Getal[Cijfer][i][j]);                                       // Reading a byte of data stored in program memory (PROGMEM area) at a specified address.
    if ( c )                                                                                  // if Digit == 1 then turn that light on
     {                                                                                        // Serial.print(strip.getPixelColor(LEDnum) & 0X00FFFFFF,HEX); Serial.print(" ");
      if((Pos_Y+j)%2) LEDnum = ((MATRIX_WIDTH -1) - (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT));
      else            LEDnum =                      (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT); 
      StripGetPixelColor(LEDnum) && white ? OrgColor = LetterColor : OrgColor = 0;
      ColorLeds("",  LEDnum, LEDnum, (uint32_t)(OrgColor + white));
     }
   }
  }
}
//--------------------------------------------                                                //
//  LED Time in four digits in display
//--------------------------------------------
void TimePlaceDigit(byte uur, byte minuut)
{   
 Zet_Pixel(    uur / 10, 2, 1);  Zet_Pixel(    uur % 10, 7, 1);
 Zet_Pixel( minuut / 10, 2, 7);  Zet_Pixel( minuut % 10, 7, 7);
}

//--------------------------------------------                                                //
//  LED In- or decrease light intensity value i.e. Slope
//--------------------------------------------
void WriteLightReducer(int amount)
{
 int value = Mem.LightReducer + amount;                                                       // Prevent byte overflow by making it an integer before adding
 Mem.LightReducer = (byte) min(value,  255);                                                         // May not be larger than 255
 sprintf(sptext,"Max brightness: %3d%%",Mem.LightReducer);
 Tekstprintln(sptext);
}
//--------------------------- Time functions --------------------------
    
//--------------------------------------------                                                //
// RTC Get time from NTP cq internal ESP32 RTC 
// and store it in timeinfo struct
// return local time in unix time format
//--------------------------------------------
time_t GetTijd(bool printit)
{
 time_t now;
 
 if (Mem.UseDS3231) GetDS3231Time(false);                                                     // If the DS3231 is attached and used get its time in timeinfo struct
 else
    { 
     if(Mem.NTPOn)  getLocalTime(&timeinfo);                                                  // If NTP is running get the local time
     else { time(&now); localtime_r(&now, &timeinfo);}                                        // Else get the time from the internal RTC 
    }
 if (printit)  PrintRTCTime();                                                                // 
  localtime(&now);
 return now;                                                                                  // Return the unixtime in seconds
}


//--------------------------------------------                                                //
// NTP print the NTP time for the timezone set 
//--------------------------------------------
void GetNTPtime(bool printit)
{
 NTP.getTime();                                                                               // Force a NTP time update 
 if(printit) PrintNTPtime();
}
//--------------------------------------------                                                //
// NTP print the NTP time for the timezone set 
//--------------------------------------------
void PrintNTPtime(void)
{
 sprintf(sptext,"%s  ", NTP.getTimeDateString());  
 Tekstprint(sptext);              // 17/10/2022 16:08:15
}

//--------------------------------------------                                                //
// NTP print the NTP UTC time 
//--------------------------------------------
void PrintUTCtime(void)
{
 time_t tmi;
 struct tm* UTCtime;
 time(&tmi);
 UTCtime = gmtime(&tmi);
 sprintf(sptext,"UTC: %02d:%02d:%02d %02d-%02d-%04d  ", 
     UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec,
     UTCtime->tm_mday,UTCtime->tm_mon+1,UTCtime->tm_year+1900);
 Tekstprint(sptext);   
}

//--------------------------------------------                                                //
// DS3231 Init module
//--------------------------------------------
void InitDS3231Mod(void)
{
 DS3231Installed = IsDS3231I2Cconnected();                                                    // Check if DS3231 is connected and working   
 sprintf(sptext,"External RTC module %s found", DS3231Installed?"IS":"NOT");
 RTCklok.begin();     
 Tekstprintln(sptext);                                                                 
}
//--------------------------------------------                                                //
// DS3231 check for I2C connection
// DS3231_I2C_ADDRESS (= often 0X68) = DS3231 module
//--------------------------------------------
bool IsDS3231I2Cconnected(void)
 {
  bool DS3231Found = false;
  for (byte i = 1; i < 120; i++)
  {
   Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)                                                       
      {
      sprintf(sptext,"Found I2C address: 0X%02X", i); Tekstprintln(sptext);  
      if( i== DS3231_I2C_ADDRESS) DS3231Found = true;
      } 
  }
  return DS3231Found;   
  }
//--------------------------------------------                                                //
// DS3231 Get temperature from DS3231 module
//--------------------------------------------
float GetDS3231Temp(void)
{
 byte tMSB, tLSB;
 float temp3231;
 
  Wire.beginTransmission(DS3231_I2C_ADDRESS);                                                 // Temp registers (11h-12h) get updated automatically every 64s
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
 
  if(Wire.available()) 
  {
    tMSB = Wire.read();                                                                       // 2's complement int portion
    tLSB = Wire.read();                                                                       // fraction portion 
    temp3231 = (tMSB & 0b01111111);                                                           // do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ) + 0.5;                                                 // only care about bits 7 & 8 and add 0.5 to round off to integer   
  }
  else   {temp3231 = -273; }  
  return (temp3231);
}

//--------------------------------------------                                                //
// DS3231 Set time in module DS3231
//--------------------------------------------
void SetDS3231Time(void)
{
RTCklok.adjust(DateTime(timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, 
                        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
}

//--------------------------------------------                                                //
// DS3231 reads time in module DS3231
// and store it in Word clock time structure
//--------------------------------------------
void GetDS3231Time(bool printit)
{
 Inow             = RTCklok.now();                                                           // Be sure to get the lates DS3231 RTC clock time
 timeinfo.tm_hour = Inow.hour();
 timeinfo.tm_min  = Inow.minute();
 timeinfo.tm_sec  = Inow.second();
 timeinfo.tm_year = Inow.year() - 1900;                                                      // Inow.year() is years since 2000 tm_year is years since 1900
 timeinfo.tm_mon  = Inow.month() - 1;
 timeinfo.tm_mday = Inow.day();
 if (printit) PrintRTCTime(); 
}

//--------------------------------------------                                                //
// DS3231 prints time to serial
// reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
//--------------------------------------------
void PrintDS3231Time(void)
{
 Inow = RTCklok.now();                                                                        // Be sure to get the lates DS3231 RTC clock time
 sprintf(sptext,"%02d/%02d/%04d %02d:%02d:%02d ", Inow.day(),Inow.month(),Inow.year(),
                                                  Inow.hour(),Inow.minute(),Inow.second());
 Tekstprint(sptext);
}

//--------------------------------------------                                                //
// RTC prints the ESP32 internal RTC time to serial
//--------------------------------------------
void PrintRTCTime(void)
{
 sprintf(sptext,"%02d/%02d/%04d %02d:%02d:%02d ", 
     timeinfo.tm_mday,timeinfo.tm_mon+1,timeinfo.tm_year+1900,
     timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 Tekstprint(sptext);
}
//--------------------------------------------                                                //
// RTC Fill sptext with time
//--------------------------------------------
void PrintTimeHMS(){ PrintTimeHMS(2);}                                                        // print with linefeed
void PrintTimeHMS(byte format)
{
 sprintf(sptext,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 switch (format)
 {
  case 0: break;
  case 1: Tekstprint(sptext); break;
  case 2: Tekstprintln(sptext); break;  
 }
}

//--------------------------------------------                                                //
// RTC Set time from global timeinfo struct
// Check if values are within range
//--------------------------------------------
void SetRTCTime(void) 
{ 
 time_t t = mktime(&timeinfo);                                                                // t= unixtime
 setRTCTime(t);
}  
void setRTCTime(time_t t)
{ 
 // time_t t = mktime(&timeinfo);  // t= unixtime
 sprintf(sptext, "Setting time: %s", asctime(&timeinfo));    Tekstprintln(sptext);
 struct timeval now = { .tv_sec = t , .tv_usec = 0};
 settimeofday(&now, NULL);
 GetTijd(false);                                                                                  // Synchronize time with RTC clock
 Displaytime();
 PrintTimeHMS();
}

//--------------------------------------------                                                //
// Print all the times available 
//--------------------------------------------
void PrintAllClockTimes(void)
{
 Tekstprint(" Clock time: ");
 PrintRTCTime();
 //GetTijd(1);
 Tekstprint("\n   NTP time: ");
 PrintNTPtime();
 if(DS3231Installed)
   {
    Tekstprint("\nDS3231 time: ");
    PrintDS3231Time();
   }
 Tekstprintln(""); 
}
//                                                                                            //
// ------------------- End  Time functions 
//--------------------------------------------                                                //
//  CLOCK Convert Hex to uint32
//--------------------------------------------
uint32_t HexToDec(String hexString) 
{
 uint32_t decValue = 0;
 int nextInt;
 for (uint8_t i = 0; i < hexString.length(); i++) 
  {
   nextInt = int(hexString.charAt(i));
   if (nextInt >= 48 && nextInt <= 57)  nextInt = map(nextInt, 48, 57, 0, 9);
   if (nextInt >= 65 && nextInt <= 70)  nextInt = map(nextInt, 65, 70, 10, 15);
   if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
   nextInt = constrain(nextInt, 0, 15);
   decValue = (decValue * 16) + nextInt;
  }
 return decValue;
}
//------------------------------------------------------------------------------
// CLOCK Demo mode
//------------------------------------------------------------------------------
//                                                                                            //
void Demomode(void)
{
 if ( millis() - msTick == 10)   digitalWrite(secondsPin,LOW);                                // Turn OFF the second on pin 13
 if ( millis() - msTick >= MilliSecondValue)                                                  // Flash the onboard Pin 13 Led so we know something is happening
 {    
  msTick = millis();                                                                          // second++; 
  digitalWrite(secondsPin,HIGH);                                                              // Turn ON the second on pin 13
  if( ++timeinfo.tm_sec >59) { timeinfo.tm_sec = 0; }
  if( ++timeinfo.tm_min >59) { timeinfo.tm_min = 0; timeinfo.tm_sec = 0; timeinfo.tm_hour++;}
  if( timeinfo.tm_hour >24) timeinfo.tm_hour = 0;                                             // If hour is after 12 o'clock 
  DimLeds(false);
                        #ifdef RGBCLOCK
   SetSecondColour();                                                                          // Set the colour per second of 'IS' and 'WAS' 
                       #endif
  Displaytime();
  Tekstprintln("");
  SerialCheck();
 }
}

//--------------------------------------------                                                //
// BLE 
// SendMessage by BLE Slow in packets of 20 chars
// or fast in one long string.
// Fast can be used in IOS app BLESerial Pro
//------------------------------
void SendMessageBLE(std::string Message)
{
 if(deviceConnected) 
   {
    if (Mem.UseBLELongString)                                                                 // If Fast transmission is possible
     {
      pTxCharacteristic->setValue(Message); 
      pTxCharacteristic->notify();
      delay(10);                                                                              // Bluetooth stack will go into congestion, if too many packets are sent
     } 
   else                                                                                       // Packets of max 20 bytes
     {   
      int parts = (Message.length()/20) + 1;
      for(int n=0;n<parts;n++)
        {   
         pTxCharacteristic->setValue(Message.substr(n*20, 20)); 
         pTxCharacteristic->notify();
         delay(40);                                                                           // Bluetooth stack will go into congestion, if too many packets are sent
        }
     }
   } 
}
//-----------------------------
// BLE Start BLE Classes
//------------------------------
class MyServerCallbacks: public BLEServerCallbacks 
{
 void onConnect(BLEServer* pServer) {deviceConnected = true; };
 void onDisconnect(BLEServer* pServer) {deviceConnected = false;}
};

class MyCallbacks: public BLECharacteristicCallbacks 
{
 void onWrite(BLECharacteristic *pCharacteristic) 
  {
   std::string rxValue = pCharacteristic->getValue();
   ReceivedMessageBLE = rxValue + "\n";
//   if (rxValue.length() > 0) {for (int i = 0; i < rxValue.length(); i++) printf("%c",rxValue[i]); }
//   printf("\n");
  }  
};
//--------------------------------------------                                                //
// BLE Start BLE Service
//------------------------------
void StartBLEService(void)
{
 BLEDevice::init(Mem.BLEbroadcastName);                                                       // Create the BLE Device
 pServer = BLEDevice::createServer();                                                         // Create the BLE Server
 pServer->setCallbacks(new MyServerCallbacks());
 BLEService *pService = pServer->createService(SERVICE_UUID);                                 // Create the BLE Service
 pTxCharacteristic                     =                                                      // Create a BLE Characteristic 
     pService->createCharacteristic(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);                 
 BLECharacteristic * pRxCharacteristic = 
     pService->createCharacteristic(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
 pRxCharacteristic->setCallbacks(new MyCallbacks());
 pService->start(); 
 BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
 pAdvertising->addServiceUUID(SERVICE_UUID); 
 pServer->start();                                                                            // Start the server  Nodig??
 pServer->getAdvertising()->start();                                                          // Start advertising
 TekstSprint("BLE Waiting a client connection to notify ...\n"); 
}
//                                                                                            //
//-----------------------------
// BLE  CheckBLE
//------------------------------
void CheckBLE(void)
{
 if(!deviceConnected && oldDeviceConnected)                                                   // Disconnecting
   {
    delay(300);                                                                               // Give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();                                                              // Restart advertising
    TekstSprint("Start advertising\n");
    oldDeviceConnected = deviceConnected;
   }
 if(deviceConnected && !oldDeviceConnected)                                                   // Connecting
   { 
    oldDeviceConnected = deviceConnected;
    SWversion();
   }
 if(ReceivedMessageBLE.length()>0)
   {
    SendMessageBLE(ReceivedMessageBLE);
    String BLEtext = ReceivedMessageBLE.c_str();
    ReceivedMessageBLE = "";
    ReworkInputString(BLEtext); 
   }
}

//--------------------------------------------                                                //
// WIFI WIFIEvents
//--------------------------------------------
void WiFiEvent(WiFiEvent_t event)
{
  sprintf(sptext,"[WiFi-event] event: %d  : ", event); 
  Tekstprint(sptext);
  WiFiEventInfo_t info;
    switch (event) 
     {
        case ARDUINO_EVENT_WIFI_READY: 
            Tekstprintln("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Tekstprintln("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Tekstprintln("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Tekstprintln("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            sprintf(sptext,"Connected to %s. Asking for IP address.\r\n", Mem.Ssid);  // info.wifi_sta_connected.ssid);
            Tekstprint(sptext);
            break;
       case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Mem.ReconnectWIFI++;
            sprintf(sptext, "Disconnected from station, attempting reconnection for the %d time", Mem.ReconnectWIFI);
            Tekstprintln(sptext);
            sprintf(sptext,"WiFi lost connection.");                                          // Reason: %d",info.wifi_sta_disconnected.reason); 
            Tekstprintln(sptext);
            WiFi.reconnect();
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Tekstprintln("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            sprintf(sptext, "Obtained IP address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
            Tekstprintln(sptext);
            WiFiGotIP(event,info);
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Tekstprintln("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
           sprintf(sptext, "WPS Successfull, stopping WPS and connecting to: %s: ", WiFi.SSID().c_str());
           Tekstprintln(sptext);
          //  wpsStop();
          //  delay(10);
          //  WiFi.begin();
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Tekstprintln("WPS Failed, retrying");
//            wpsStop();
//            wpsStart();
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Tekstprintln("WPS Timedout, retrying");
//            wpsStop();
//            wpsStart();
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
 //           txtstr = wpspin2string(info.wps_er_pin.pin_code).c_str();
//            sprintf(sptext,"WPS_PIN = %s",txtstr);
//            Tekstprintln(sptext);
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Tekstprintln("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Tekstprintln("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Tekstprintln("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            sprintf(sptext,"Client disconnected.");                                            // Reason: %d",info.wifi_ap_stadisconnected.reason); 
            Tekstprintln(sptext);

//          Serial.print("WiFi lost connection. Reason: ");
//          Tekstprintln(info.wifi_sta_disconnected.reason);
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Tekstprintln("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Tekstprintln("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Tekstprintln("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Tekstprintln("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Tekstprintln("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            Tekstprintln("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Tekstprintln("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Tekstprintln("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Tekstprintln("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Tekstprintln("Obtained IP address");
            WiFiGotIP(event,info);
            break;
        default: break;
    }
}
//--------------------------------------------                                                //
// NTP Check NTP SyncEvent 
//--------------------------------------------
void NTPsyncEvent(void)
{
    if (syncEventTriggered) 
    {
      syncEventTriggered = false;
      processSyncEvent (ntpEvent);
    }
}
//--------------------------------------------                                                //
// NTP processSyncEvent 
//--------------------------------------------
void processSyncEvent (NTPEvent_t ntpEvent) 
{
 switch (ntpEvent.event) 
    {
        case timeSyncd:
        case partlySync:
        case syncNotNeeded:
        case accuracyError:
            sprintf(sptext,"[NTP-event] %s", NTP.ntpEvent2str (ntpEvent));
            Tekstprintln(sptext);
            break;
        default:
            break;
    }
}
//--------------------------------------------                                                //
// WIFI GOT IP address 
//--------------------------------------------
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  WIFIConnected = 1;
 if(Mem.WIFIOn) WebPage();                                                                    // Show the web page if WIFI is on
 if(Mem.NTPOn)
   {
    NTP.setTimeZone(Mem.Timezone);                                                            // TZ_Europe_Amsterdam); //\TZ_Etc_GMTp1); // TZ_Etc_UTC 
    NTP.begin();                                                                              // https://raw.githubusercontent.com/nayarsystems/posix_tz_db/master/zones.csv
    Tekstprintln("NTP is On"); 
   } 
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void StartWIFI_NTP(void)
{
// WiFi.disconnect(true);
 WiFi.onEvent(WiFiEvent);   // Using WiFi.onEvent interrupts and crashes IL9341 screen display while writing the screen
// Examples of different ways to register wifi events
                         //  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
                         //  WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                         //    {Serial.print("WiFi lost connection. Reason: ");  Serial.println(info.wifi_sta_disconnected.reason); }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
// if(strlen (Mem.Ssid) <5 || strlen(Mem.Password) <6) { AskSSIDPW(); return;}

// WiFi.mode(WIFI_STA);
 WiFi.begin(Mem.Ssid, Mem.Password); 
 if (WiFi.waitForConnectResult() != WL_CONNECTED) 
      { 
       if(Mem.WIFINoOfRestarts == 0)                                                           // Try once to restart the ESP and make a new WIFI connection
       {
        Mem.WIFINoOfRestarts = 1;
        StoreStructInFlashMemory();                                                            // Update Mem struct   
        ESP.restart(); 
       }
       else
       {  
        Tekstprintln("WiFi Failed! Enter @ to retry"); 
        WIFIConnected = 0;       
        return;
       }
      }
 else 
      {
       Tekstprint("Web page started\n");
       sprintf(sptext, "IP Address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
       Tekstprintln(sptext); 
       WIFIConnected = 1;
       Mem.WIFINoOfRestarts = 0;
       StoreStructInFlashMemory();                                                            // Update Mem struct   
       ElegantOTA.begin(&server);                                                             // Start ElegantOTA  new version in 2023  
       if(Mem.NTPOn)
          {
           NTP.setTimeZone(Mem.Timezone);                                                     // TZ_Europe_Amsterdam); //\TZ_Etc_GMTp1); // TZ_Etc_UTC 
           NTP.begin();                                                                       // https://raw.githubusercontent.com/nayarsystems/posix_tz_db/master/zones.csv
           NTP.onNTPSyncEvent([](NTPEvent_t event){ntpEvent=event; syncEventTriggered=true;});
           Tekstprintln("NTP is On"); 
          }
      }
 if(Mem.WIFIOn) WebPage();                                                                    // Show the web page if WIFI is on
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void WebPage(void) 
{
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)     
          { request->send(200, "text/html", index_html);  }    );                             // Send web page with input fields to client
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)
       { 
        String inputMessage;    String inputParam;
        if (request->hasParam(PARAM_INPUT_1))                                                 // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
           {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            inputParam = PARAM_INPUT_1;
           }
        else 
           {
            inputMessage = "";    //inputMessage = "No message sent";
            inputParam = "none";
           }  
//      sprintf(sptext,"%s",inputMessage.c_str());    Tekstprintln(sptext); 
        ReworkInputString(inputMessage);
        request->send(200, "text/html", index_html);
       }   );                                                                                  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
 server.onNotFound(notFound);
 server.begin();
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void notFound(AsyncWebServerRequest *request) 
{
  request->send(404, "text/plain", "Not found");
}


//--------------------------------------------                                                //
// DCF77 Start the module
//--------------------------------------------
void StartDCF77(void)
{
                                 #ifdef DCF77MOD 
 pinMode(DCF_LED_Pin,  OUTPUT);                                                               // Pin of the LED that displays the received signal
 pinMode(DCF_PIN,INPUT_PULLUP);                                                               // Pin the DCF77 is connected to
 DCF.Start();                                                                                 // Start the DCF-module
                                 #endif //DCF77MOD 
}
 
                                 #ifdef DCF77MOD
//--------------------------------------------                                                //
// DCF77 check for DCF input
//--------------------------------------------
void DCF77Check(void)
{
 time_t DCFtime = DCF.getTime();                                                              // Check if new DCF77 time is available
 if (DCFtime!=0)                                                                              // If a valid time/date is received ...
  {
   DCF_signal+=2;                                                                             // Add 2 to the counter. NB 1 is subtracted every minute 
   DCF_counts++;                                                                              // Valid DCF receptions per hour
 //  setTime(DCFtime);                                                                          // Update the internal time of the Arduino (Used by the DCF library) RTC time
   
   Inow = RTCklok.now();                                                                      // Update the internal time in the variabele of this program
   Tekstprint("DCF OK: ");
   if(_abs(DCFtime - Inow.unixtime()) > 2)                                                     // If the time differs more than a second -> update

      {
       RTCklok.adjust(DCFtime);                                                               // Update the  RTC time
       Tekstprintln("Time NOK updated");
       PrintRTCTime();
      }
   else Tekstprintln("Time OK");
  }
 bool LHbit = digitalRead(DCF_PIN);
 if (UseDCF)  digitalWrite(DCF_LED_Pin, 1 - LHbit );                                          // Write inverted DCF pulse to LED on board 
 if (SeeDCFsignalInDisplay)
  {
   Toggle_HetWasIs = LHbit;
   Display1 |= (Toggle_HetWasIs<<0);                                                          // Turn off the  HET IS WAS LEDs
   Displaytime();
  }
  DCF_signal = _min(DCF_signal, 100);                                                          // Keep DCF_signal counter <=100
} 
                                  #endif //DCF77MOD                            
                                  #ifdef ONEWIREKEYPAD3x4
//--------------------------------------------                                                //
// KEYPAD check for Onewire Keypad input
//--------------------------------------------
void OnewireKeypad3x4Check(void)
{
 int keyvalue=99;;
 int Key=0;
 int sensorValue = analogRead(OneWirePin); // read the value from the sensor:
 switch(sensorValue)
  {
    case   0 ... 100:  keyvalue = 13; break;   // noise
    case 101 ... 132:  keyvalue = 12; Key = '*'; break;   // * 
    case 133 ... 154:  keyvalue =  0; Key = '0'; break;   // 0 
    case 155 ... 216:  keyvalue = 11; Key = '#'; break;   // # 
    case 217 ... 281:  keyvalue =  7; Key = '7'; break;   // 7 
    case 282 ... 318:  keyvalue =  4; Key = '4'; break;   // 4 
    case 319 ... 349:  keyvalue =  1; Key = '1'; break;   // 1 
    case 350 ... 390:  keyvalue =  8; Key = '8'; break;   // 8 
    case 391 ... 463:  keyvalue =  5; Key = '5'; break;   // 5 
    case 464 ... 519:  keyvalue =  2; Key = '2'; break;   // 2 
    case 520 ... 619:  keyvalue =  9; Key = '9'; break;   // 9 
    case 620 ... 848:  keyvalue =  6; Key = '6'; break;   // 6 
    case 849 ... 1023: keyvalue =  3; Key = '3'; break;   // 3
  }
 if(keyvalue<13) { Serial.println(Key); delay(300); }
  if (Key == 12)   // *                                                                       // Pressing a * activates the keyboard input. 
   { 
    KeyInputactivated = true;
    KeyLooptime = millis();
    KeypadString ="";
    ColorLeds("",0,NUM_LEDS-1,0x00FF00);                                                      // Turn all LEDs green
    ShowLeds();                                                                               // Push data in LED strip to commit the changes
    Serial.println(F("Key entry activated"));
   }
 if (KeyInputactivated && (Key>=0 && Key<10))
   {
    delay(20); 
    KeypadString += Key;                                                                      // Digit keys 0 - 9
    ColorLeds("",0,Key-48,0xFF0000);                                                          // Turn all LEDs red
    ShowLeds();                                                                               // Push data in LED strip to commit the changes
 //   Serial.println(KeypadString);
   }
 if (KeypadString.length()>5)                                                                 // If six numbers are entered rework this to a time hhmmss
   {       
   if(KeypadString=="999999")
     { 
      KeypadString = "";   
      Reset();
      Serial.println(F("Settings reset"));   
     }
    else 
     {      
      ReworkInputString(KeypadString);                                                        // Rework ReworkInputString();
      KeypadString = "";
      Serial.println(F("Time changed"));
     }    
   }
 if ( KeyInputactivated && ((millis() - KeyLooptime) > 30000) ) 
   {  
    KeyInputactivated = false;                                                                // Stop data entry after 30 seconds. This avoids unintended entry 
    KeypadString ="";
    Serial.println(F("Keyboard entry stopped"));
  }
}
                                  #endif  //ONEWIREKEYPAD3x4  
                                  #ifdef ONEWIREKEYPAD3x1
//--------------------------------------------                                                //
// KEYPAD check for Onewire Keypad input with 5V and 1.1, 4.7, 4.7, 4.7 kOhm resistors
//--------------------------------------------
void OnewireKeypad3x1Check(void)
{
 int8_t keyvalue = 99;
 int8_t Key;
 int16_t sensorValue = analogRead(OneWirePin);                                                    // Read the value from the sensor:
 switch(sensorValue)
   {
    case   0 ... 385:  keyvalue = 99;            break;                                       // Noise
    case 386 ... 635:  keyvalue = -1; Key = 'G'; break;                                       // G 
    case 636 ... 910:  keyvalue =  0; Key = 'Y'; break;                                       // Y 
    case 911 ... 1024: keyvalue =  1; Key = 'R'; break;                                       // R 
   }
 if(keyvalue<2) 
    { 
//     Serial.print(sensorValue); Serial.println(Key); 
     if (Key == 'R') ProcessKeyPressTurn(1);                                                  // Pressing Red increases hour or minute. 
     if (Key == 'G') ProcessKeyPressTurn(-1);                                                 // Pressing Green decreases hour or minute. 
     if (Key == 'Y') ProcessKeyPressTurn(0);                                                  // Pressing Yellow activates the keyboard input. 
     delay(200);     
    }
}
                                  #endif //ONEWIREKEYPAD3x1

                                 #ifdef ROTARYMOD
//--------------------------------------------                                                //
// KY-040 ROTARY check if the rotary is moving
//--------------------------------------------
void RotaryEncoderCheck(void)
{
 int ActionPress = 999;
 if (digitalRead(clearButton) == LOW )          ProcessKeyPressTurn(0);                       // Set the time by pressing rotary button
 else if (ChangeTime)    
  {   
   ActionPress = myEnc.read();                                                                // If the knob is turned store the direction (-1 or 1)
   if (ActionPress == 0) {  ActionPress = 999;  ProcessKeyPressTurn(ActionPress);  }          // Sent 999 = nop 
   if (ActionPress == 1 || ActionPress == -1 )  ProcessKeyPressTurn(ActionPress);             // Process the ActionPress
  } 
 myEnc.write(0);                                                                              // Set encoder pos back to 0
}
                                  #endif  //ROTARYMOD  

//--------------------------------------------                                                //
// CLOCK
// KY-040 or Membrane 3x1 processing input
// encoderPos < 1 left minus 
// encoderPos = 0 attention and selection choice
// encoderPos > 1 right plus
//--------------------------------------------
void ProcessKeyPressTurn(int encoderPos)
{
if ((unsigned long) (millis() - RotaryPressTimer) > 60000)                                    // After 60 sec after shaft is pressed time of light intensity can not be changed 
   {
    if (ChangeTime || ChangeLightIntensity)                         
      {
        Tekstprintln("<-- Changing time is over -->");
        NoofRotaryPressed = 0;
      }
    ChangeTime            = false;
    ChangeLightIntensity  = false;
   }  
 if (ChangeTime || ChangeLightIntensity)                                                      // If shaft is pressed time of light intensity can be changed
   {
    if ( encoderPos!=999 && ( (millis() - Looptime) > 250))                                   // If rotary turned avoid debounce within 0.25 sec
     {   
     Serial.print(F("----> Index:"));   Serial.println(encoderPos);
     if (encoderPos == 1)                                                                     // Increase  
       {     
        if (ChangeLightIntensity)  { WriteLightReducer(5); }                                  // If time < 60 sec then adjust light intensity factor
        if (ChangeTime) 
          {
           if (NoofRotaryPressed == 1)                                                        // Change hours
              {if( ++timeinfo.tm_hour >23) { timeinfo.tm_hour = 0; } }      
           if (NoofRotaryPressed == 2)                                                        // Change minutes
              {  timeinfo.tm_sec = 0;
               if( ++timeinfo.tm_min >59) { timeinfo.tm_min = 0; if( ++timeinfo.tm_hour >23) { timeinfo.tm_hour = 0; } }   }
           } 
        }    
      if (encoderPos == -1)                                                                   // Decrease
       {
       if (ChangeLightIntensity)   { WriteLightReducer(-5); }    // If time < 60 sec then adjust light intensity factor
       if (ChangeTime)     
          {
           if (NoofRotaryPressed == 1)                                                        // Change hours
            { if( timeinfo.tm_hour-- ==0) { timeinfo.tm_hour = 23; } }      
           if (NoofRotaryPressed == 2)                                                        // Change minutes
            { timeinfo.tm_sec = 0;
             if( timeinfo.tm_min-- == 0) { timeinfo.tm_min = 59; if( timeinfo.tm_hour-- == 0) { timeinfo.tm_hour = 23; } }  }
          }          
        } 
      SetDS3231Time();  
      PrintDS3231Time();
      Looptime = millis();       
     }                                                
   }
 if (encoderPos == 0 )                                                                        // Set the time by pressing rotary button
   { 
    delay(250);
    ChangeTime            = false;
    ChangeLightIntensity  = false;
    SeeDCFsignalInDisplay = false;                                                            // Shows the DCF-signal NOT in the display
    RotaryPressTimer      = millis();                                                         // Record the time the shaft was pressed.
    if(++NoofRotaryPressed >6 ) NoofRotaryPressed = 0;
    switch (NoofRotaryPressed)                                                                // No of times the rotary is pressed
      {
       case 1:  ChangeTime = true;           BlinkUUR(3, 20);       break;                    // Change the hours
       case 2:  ChangeTime = true;           BlinkHETISWAS(3, 20);  break;                    // Change the hours        
       case 3:  ChangeLightIntensity = true; BlinkTWAALF(3, 20);    break;                    // Turn on TWAALF and change intensity 
       case 4:  SeeDCFsignalInDisplay = true;                       break;                    // Shows the DCF-signal in the display                               
       case 5:                                                      break;
       case 6:  Reset();                                            break;                    // Reset all settings                                                                  
      default:                                                      break;                     
      }
    Serial.print(F("NoofRotaryPressed: "));   Serial.println(NoofRotaryPressed);   
    Looptime = millis();     
    Displaytime();                                                                            // Turn on the LEDs with proper time
   }
 }

//--------------------------------------------                                                //
//  HC595 LED Clear display settings of the LED's
//--------------------------------------------
void LedsOffBW(void){  Display1=0;  Display2=0;  Display3=0; }
//--------------------------------------------                                                //
//  HC595 LED Turn On the LED's
//  Write the actual values to the hardware 
//--------------------------------------------
void WriteLEDs(void) 
{                                                                                
 digitalWrite(LEDStrobePin,LOW);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display3);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display2);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display1);
 digitalWrite(LEDStrobePin,HIGH);
 delay(2);
} 
//--------------------------------------------                                                //
// HC595 Update routine done every second
//--------------------------------------------
void HC595bank(uint8_t value) 
{
  uint8_t Banknr = value / 8;
  uint8_t Bitnr  = value % 8;  
  Display1 = Display2 = Display3 = 0;
  if(Banknr == 0)  Display1 = (1 << Bitnr);  
  if(Banknr == 1)  Display2 = (1 << Bitnr); 
  if(Banknr == 2)  Display3 = (1 << Bitnr); 
  digitalWrite(LEDStrobePin,LOW);
  shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display3);
  shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display2);
  shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display1);
  digitalWrite(LEDStrobePin,HIGH);
}
//                                                                                            //
