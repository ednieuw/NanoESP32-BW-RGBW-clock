//------------------------------------------------------------------------------
// ARDUINO Definition of installed White LED language word clock
// These clocks have a 11 x 11 character matrix
//------------------------------------------------------------------------------

extern void CheckDevices(void);   
extern void ShiftInTime(const char TimeText[10], byte num , byte Displaynr, byte Shiftbit, byte ShiftIn);
extern void LedsOffBW(void); 
extern void WriteLEDs(void);
extern void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor);
extern bool LEDsAreOff;
extern int  ToggleEdsoft;
extern byte Toggle_HetWasIs;
extern struct tm timeinfo;                                                                 // Storage of time used in clock for time display

void Laatzien(void);
void Laatzien(int Delaymsec); 

const uint32_t NUM_LEDS  =   4;                                                            // 4 RGB(W) LEDs for the minutes
const byte MATRIX_WIDTH  =  12;                                                            // NA Not used in this clock. 12 is fine 
const byte MATRIX_HEIGHT =  12;                                                            // NA Not used in this clock. 12 is fine 

//------------------------------------------------------------------------------
// RGBW LEDs Definition of LEDs to be used for minutes.
// Normally the number start at the end of the last LED used.
// In this case 139
// These LEDs can also be used in the white LED clock
//------------------------------------------------------------------------------
                     #ifdef BWCLOCK
#define MIN0   ColorLeds("M0",    NUM_LEDS-4, NUM_LEDS-1, black); 
#define MIN1   ColorLeds("M1",    NUM_LEDS-4, NUM_LEDS-4, white); 
#define MIN2   ColorLeds("M2",    NUM_LEDS-3, NUM_LEDS-3, white); 
#define MIN3   ColorLeds("M3",    NUM_LEDS-2, NUM_LEDS-2, white);
#define MIN4   ColorLeds("M4",    NUM_LEDS-1, NUM_LEDS-1, white);     
                     #endif // BWCLOCK


            // new lay-out   
                     #ifdef NLBW
#define HET     ShiftInTime("Het ",    0, 1,Toggle_HetWasIs,0);
#define IS      ShiftInTime("is ",     1, 1,Toggle_HetWasIs,1);
#define WAS     ShiftInTime("was ",    2, 1,Toggle_HetWasIs,2);
#define MVIJF   ShiftInTime("vijf ",   3, 1,              1,3);
#define MTIEN   ShiftInTime("tien ",   4, 1,              1,4);
#define KWART   ShiftInTime("kwart ",  5, 1,              1,5);
#define VOOR    ShiftInTime("voor ",   6, 1,              1,6);
#define OVER    ShiftInTime("over ",   7, 1,              1,7);

#define PRECIES ShiftInTime("precies ",8, 2,              1,0);
#define HALF    ShiftInTime("half ",   9, 2,              1,1);
#define ELF     ShiftInTime("elf ",   10, 2,              1,2);
#define VIJF    ShiftInTime("vijf ",  11, 2,              1,3);
#define TWEE    ShiftInTime("twee ",  12, 2,              1,4);
#define EEN     ShiftInTime("een ",   13, 2,              1,5);
#define VIER    ShiftInTime("vier ",  14, 2,              1,6);
#define TIEN    ShiftInTime("tien ",  15, 2,              1,7);

#define TWAALF  ShiftInTime("twaalf ",16, 3,              1,0);
#define DRIE    ShiftInTime("drie ",  17, 3,              1,1);
#define NEGEN   ShiftInTime("negen ", 18, 3,              1,2);
#define ACHT    ShiftInTime("acht ",  19, 3,              1,3);
#define ZES     ShiftInTime("zes ",   20, 3,              1,4);
#define ZEVEN   ShiftInTime("zeven ", 21, 3,              1,5);
#define UUR     ShiftInTime("uur ",   22, 3,              1,6);
#define EDSOFT  ShiftInTime("Edsoft ",23, 3, ToggleEdsoft  ,7);
                     #endif //NLBW

                     #ifdef UKBW
#define HET     ShiftInTime("It ",     0, 1,Toggle_HetWasIs,0);
#define IS      ShiftInTime("is ",     1, 1,Toggle_HetWasIs,0);
#define WAS     ShiftInTime("was ",    2, 1,Toggle_HetWasIs,0);
#define PRECIES ShiftInTime("exact "  ,3, 1,              1,3);
#define HALF    ShiftInTime("half ",   4, 1,              1,4);
#define TWINTIG ShiftInTime("twenty ", 5, 1,              1,5);
#define MVIJF   ShiftInTime("five ",   6, 1,              1,6);
#define KWART   ShiftInTime("quarter ",7, 1,              1,7);

#define MTIEN   ShiftInTime("ten ",    8, 2,              1,0);
#define OVER    ShiftInTime("past ",   9, 2,              1,1);
#define ZES     ShiftInTime("six ",   10, 2,              1,2);
#define TWEE    ShiftInTime("two ",   11, 2,              1,3);
#define VIJF    ShiftInTime("five ",  12, 2,              1,4);
#define TWAALF  ShiftInTime("twelve ",13, 2,              1,5);
#define TIEN    ShiftInTime("ten ",   14, 2,              1,6);
#define ELF     ShiftInTime("eleven ",15, 2,              1,7);

#define VIER    ShiftInTime("four ",  16, 3,              1,0);
#define NEGEN   ShiftInTime("nine ",  17, 3,              1,1);
#define DRIE    ShiftInTime("three ", 18, 3,              1,2);
#define ACHT    ShiftInTime("eight ", 19, 3,              1,3);
#define ZEVEN   ShiftInTime("seven ", 20, 3,              1,4);
#define EEN     ShiftInTime("one ",   21, 3,              1,5);
#define UUR     ShiftInTime("O'clock",22, 3,              1,6);
#define VOOR    ShiftInTime("to ",    23, 3, ToggleEdsoft  ,7);
                     #endif //UKBW  

                     #ifdef UVBW
                // Ulrich Vertessen Clock display lay-out
#define HET     ShiftInTime("Het ",    0, 1,Toggle_HetWasIs,0);
#define IS      ShiftInTime("is ",     1, 1,Toggle_HetWasIs,1);  // Niet in klok aangesloten
#define WAS     ShiftInTime("was ",    2, 1,Toggle_HetWasIs,2);  // Niet in klok aangesloten
#define EDSOFT  ShiftInTime("Edsoft ", 2, 1, ToggleEdsoft  ,2);  // Niet in klok aangesloten
#define MVIJF   ShiftInTime("vijf ",   3, 1,              1,3);
#define MTIEN   ShiftInTime("tien ",   4, 1,              1,4);
#define KWART   ShiftInTime("kwart ",  5, 1,              1,5);
#define VOOR    ShiftInTime("voor ",   6, 1,              1,6);
#define MOVER   ShiftInTime("over ",   7, 1,              1,7);

#define OVER    ShiftInTime("over ",   8, 2,              1,0);
#define HALF    ShiftInTime("half ",   9, 2,              1,1);
#define ELF     ShiftInTime("elf ",   10, 2,              1,2);
#define VIJF    ShiftInTime("vijf ",  11, 2,              1,3);
#define TWEE    ShiftInTime("twee ",  12, 2,              1,4);
#define EEN     ShiftInTime("een ",   13, 2,              1,5);
#define VIER    ShiftInTime("vier ",  14, 2,              1,6);
#define TIEN    ShiftInTime("tien ",  15, 2,              1,7);

#define TWAALF  ShiftInTime("twaalf ",16, 3,              1,0);
#define DRIE    ShiftInTime("drie ",  17, 3,              1,1);
#define NEGEN   ShiftInTime("negen ", 18, 3,              1,2);
#define ACHT    ShiftInTime("acht ",  19, 3,              1,3);
#define ZES     ShiftInTime("zes ",   20, 3,              1,4);
#define ZEVEN   ShiftInTime("zeven ", 21, 3,              1,5);
#define UUR     ShiftInTime("uur ",   22, 3,              1,6);
#define MVOOR   ShiftInTime("voor ",  23, 3,              1,7);          

                     #endif //UVBW

//--------------------------------------------                                                //
// CLOCK set the LED's for displaying
//--------------------------------------------
void DisplayTime(void)
{
 LedsOffBW(); 
 if(LEDsAreOff) {  WriteLEDs(); return;  }                                                                                  // Start by clearing the display to a known state

                             #ifdef NLBW
  HET;                                                                                         // HET light is always on
 switch (timeinfo.tm_min)
 {
  case  0: IS;  PRECIES; break;
  case  1: IS;  break;
  case  2: 
  case  3: WAS; break;
  case  4: 
  case  5: 
  case  6: IS;  MVIJF; OVER; break;
  case  7: 
  case  8: WAS; MVIJF; OVER; break;
  case  9: 
  case 10: 
  case 11: IS;  MTIEN; OVER; break;
  case 12: 
  case 13: WAS; MTIEN; OVER; break;
  case 14: 
  case 15: 
  case 16: IS;  KWART; OVER; break;
  case 17: 
  case 18: WAS; KWART; OVER; break;
  case 19: 
  case 20: 
  case 21: IS;  MTIEN; VOOR; HALF; break;
  case 22: 
  case 23: WAS; MTIEN; VOOR; HALF; break;
  case 24: 
  case 25: 
  case 26: IS;  MVIJF; VOOR; HALF; break;
  case 27: 
  case 28: WAS; MVIJF; VOOR; HALF; break;
  case 29: IS;  HALF; break;
  case 30: IS;  PRECIES; HALF; break;
  case 31: IS;  HALF; break;
  case 32: 
  case 33: WAS; HALF; break;
  case 34: 
  case 35: 
  case 36: IS;  MVIJF; OVER; HALF; break;
  case 37: 
  case 38: WAS; MVIJF; OVER; HALF; break;
  case 39: 
  case 40: 
  case 41: IS;  MTIEN; OVER; HALF; break;
  case 42: 
  case 43: WAS; MTIEN; OVER; HALF; break;
  case 44: 
  case 45: 
  case 46: IS;  KWART; VOOR; break;
  case 47: 
  case 48: WAS; KWART; VOOR; break;
  case 49: 
  case 50: 
  case 51: IS;  MTIEN; VOOR;  break;
  case 52: 
  case 53: WAS; MTIEN; VOOR;  break;
  case 54: 
  case 55: 
  case 56: IS;  MVIJF; VOOR; break;
  case 57: 
  case 58: WAS; MVIJF; VOOR; break;
  case 59: IS;  break;
 }
byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

 switch (sayhour)
 {
  case 13:
  case  1: EEN; break;
  case 14:
  case  2: TWEE; break;
  case 15:
  case  3: DRIE; break;
  case 16:
  case  4: VIER; break;
  case 17:
  case  5: VIJF; break;
  case 18:
  case  6: ZES; break;
  case 19:
  case  7: ZEVEN; break;
  case 20:
  case  8: ACHT; break;
  case 21:
  case  9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UUR;  break; 
 }
                            #endif //NLBW

                            #ifdef UKBW
 HET;                                                                                         // HET light is always on
 switch (timeinfo.tm_min)
 {                            
  case  0: IS;  PRECIES; break;
  case  1: IS;  break;
  case  2: 
  case  3: WAS; break;
  case  4: 
  case  5: 
  case  6: IS;  MVIJF; OVER; break;
  case  7: 
  case  8: WAS; MVIJF; OVER; break;
  case  9: 
  case 10: 
  case 11: IS;  MTIEN; OVER; break;
  case 12: 
  case 13: WAS; MTIEN; OVER; break;
  case 14: 
  case 15: 
  case 16: IS;  KWART; OVER; break;
  case 17: 
  case 18: WAS; KWART; OVER; break;
  case 19: 
  case 20: 
  case 21: IS;  TWINTIG; OVER;  break;
  case 22: 
  case 23: WAS; TWINTIG; OVER; break;
  case 24: 
  case 25: 
  case 26: IS;  TWINTIG; MVIJF; OVER; break;
  case 27: 
  case 28: WAS; TWINTIG; MVIJF; OVER; break;
  case 29: IS;  HALF; OVER; break;
  case 30: IS;  PRECIES; HALF; OVER; break;
  case 31: IS;  HALF; OVER; break;
  case 32: 
  case 33: WAS; HALF; OVER; break;
  case 34: 
  case 35: 
  case 36: IS;  TWINTIG; MVIJF; VOOR; break;
  case 37: 
  case 38: WAS; TWINTIG; MVIJF; VOOR; break;
  case 39: 
  case 40: 
  case 41: IS;  TWINTIG; VOOR;  break;
  case 42: 
  case 43: WAS; TWINTIG; VOOR;  break;
  case 44: 
  case 45: 
  case 46: IS;  KWART; VOOR; break;
  case 47: 
  case 48: WAS; KWART; VOOR; break;
  case 49: 
  case 50: 
  case 51: IS;  MTIEN; VOOR;  break;
  case 52: 
  case 53: WAS; MTIEN; VOOR;  break;
  case 54: 
  case 55: 
  case 56: IS;  MVIJF; VOOR; break;
  case 57: 
  case 58: WAS; MVIJF; VOOR; break;
  case 59: IS;  break;
 }

byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

 switch (sayhour)
 {
  case 13:
  case  1: EEN; break;
  case 14:
  case  2: TWEE; break;
  case 15:
  case  3: DRIE; break;
  case 16:
  case  4: VIER; break;
  case 17:
  case  5: VIJF; break;
  case 18:
  case  6: ZES; break;
  case 19:
  case  7: ZEVEN; break;
  case 20:
  case  8: ACHT; break;
  case 21:
  case  9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UUR;  break; 
 }
                                 #endif //UKBW

                                 #ifdef UVBW

 HET; IS;                                                                                       // HET light is always on
 switch (timeinfo.tm_min)
 {
  case  0: 
  case  1: 
  case  2: 
  case  3: 
  case  4: break;
  case  5: 
  case  6:   
  case  7: 
  case  8: 
  case  9: MVIJF; MOVER; break;
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: MTIEN; MOVER; break;
  case 15: 
  case 16:
  case 17: 
  case 18: 
  case 19: KWART; OVER; break;  
  case 20:
  case 21: 
  case 22: 
  case 23: 
  case 24: MTIEN; MVOOR; HALF; break;
  case 25: 
  case 26: 
  case 27: 
  case 28: 
  case 29: MVIJF; MVOOR; HALF; break;
  case 30:
  case 31: 
  case 32: 
  case 33: 
  case 34: HALF; break;
  case 35: 
  case 36: 
  case 37: 
  case 38: 
  case 39: MVIJF; MOVER; HALF; break; 
  case 40: 
  case 41:
  case 42: 
  case 43: 
  case 44: MTIEN; MOVER; HALF; break;
  case 45: 
  case 46:
  case 47: 
  case 48: 
  case 49: KWART; VOOR; break; 
  case 50: 
  case 51: 
  case 52: 
  case 53: 
  case 54: MTIEN; VOOR;  break;
  case 55: 
  case 56:
  case 57: 
  case 58:
  case 59: MVIJF; VOOR; break;
 }


byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 19 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;
 switch (sayhour)
 {
  case 13:
  case  1: EEN; break;
  case 14:
  case  2: TWEE; break;
  case 15:
  case  3: DRIE; break;
  case 16:
  case  4: VIER; break;
  case 17:
  case  5: VIJF; break;
  case 18:
  case  6: ZES; break;
  case 19:
  case  7: ZEVEN; break;
  case 20:
  case  8: ACHT; break;
  case 21:
  case  9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 

 switch (timeinfo.tm_min)
 {
  case  0: 
  case  1: 
  case  2:
  case  3:    
  case  4: UUR;  break; 
 }
                #endif //UVBW

 switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5 
 {
  case  0: MIN0; break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }


 if(timeinfo.tm_min == 0 && timeinfo.tm_sec <9) 
 { 
  ToggleEdsoft = timeinfo.tm_sec % 2;         // ToggleEdsoft bocomes 0 or 1 and turn on and off the first 8 seconds at minute 0 the Edsoft light on pin 24
  EDSOFT;
 } 
// Tekstprintln("");
 WriteLEDs();
}

//--------------------------------------------                                                //
//  HC595 LED Turn On en Off the LED's
//--------------------------------------------
void Laatzien(int Delaymsec) 
{ 
 WriteLEDs();  
 delay(Delaymsec);  
 LedsOffBW();
 CheckDevices(); 
}

void Laatzien()
{ 
 WriteLEDs(); 
 delay(500); 
 LedsOffBW();
 CheckDevices();                                                                              // Check for input from input devices
}
// //--------------------------------------------                                                //
// //  HC595 LED  load the shiftbits in the LED display buffer
// //--------------------------------------------
// void ShiftInTime(const char TimeText[10], byte num , byte Displaynr, byte Shiftbit, byte ShiftIn)
// {  
//  if (!NoTextInLeds && strlen(Tekst) > 0 )
//      {sprintf(sptext,"%25s ",Tekst); Tekstprint(sptext); }                                      // Print the text  
//  switch (Displaynr)
//   {
//  case 1:
//         Display1 |= Shiftbit << ShiftIn;
//         break;
//  case 2:
//         Display2 |= Shiftbit << ShiftIn;
//         break;
//  case 3:
//         Display3 |= Shiftbit << ShiftIn;
//         break; 
//  }     
// }

//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------
void Selftest(void)
{ 
 LedsOffBW(); 
                 #ifdef UVBW
  HET;     Laatzien(); IS;      Laatzien(); MVIJF;   Laatzien(); 
  MTIEN;   Laatzien(); MVOOR;   Laatzien(); MOVER;   Laatzien();
  KWART;   Laatzien(); HALF;    Laatzien(); OVER;    Laatzien(); 
  VOOR;    Laatzien(); EEN;     Laatzien(); TWEE;    Laatzien();  
  DRIE;    Laatzien(); VIER;    Laatzien(); VIJF;    Laatzien(); 
  ZES;     Laatzien(); ZEVEN;   Laatzien(); NEGEN;   Laatzien(); 
  ACHT;    Laatzien(); TIEN;    Laatzien(); ELF;     Laatzien();  
  TWAALF;  Laatzien(); UUR;     Laatzien(); EDSOFT;  Laatzien();
                #endif //UVBW

                #ifdef NLBW 
  HET;     Laatzien(); IS;      Laatzien(); WAS;     Laatzien();
  PRECIES; Laatzien(); MVIJF;   Laatzien(); KWART;   Laatzien(); 
  MTIEN;   Laatzien(); OVER;    Laatzien(); VOOR;    Laatzien(); 
  HALF;    Laatzien(); TWAALF;  Laatzien(); ELF;     Laatzien();  
  EEN;     Laatzien(); TIEN;    Laatzien(); TWEE;    Laatzien(); 
  NEGEN;   Laatzien(); DRIE;    Laatzien(); ACHT;    Laatzien(); 
  VIER;    Laatzien(); ZEVEN;   Laatzien(); VIJF;    Laatzien();  
  ZES;     Laatzien(); UUR;     Laatzien(); EDSOFT;  Laatzien();
                #endif //NLBW
                #ifdef UKBW
  HET;     Laatzien();  IS;      Laatzien();  WAS;     Laatzien();
  PRECIES; Laatzien();  HALF;    Laatzien();  TWINTIG; Laatzien();
  MVIJF;   Laatzien();  KWART;   Laatzien();  MTIEN;   Laatzien();
  OVER;    Laatzien();  VOOR;    Laatzien();  ZES;     Laatzien();  
  TWEE;    Laatzien();  VIJF;    Laatzien();  TWAALF;  Laatzien();
  TIEN;    Laatzien();  ELF;     Laatzien();  VIER;    Laatzien();
  NEGEN;   Laatzien();  DRIE;    Laatzien();  ACHT;    Laatzien();
  ZEVEN;   Laatzien();  EEN;     Laatzien();  UUR;     Laatzien();
                #endif //UKBW                                                                               // Turn on the LEDs with proper time
}
