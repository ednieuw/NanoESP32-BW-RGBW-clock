//------------------------------------------------------------------------------FuncCRGBW
// RGBW LEDs Definition of installed language word clock
// These clocks have a 12 x 12 character matrix
//------------------------------------------------------------------------------

extern void Tekstprint(char const *tekst);
extern void Tekstprintln(char const *tekst);
extern void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor);
extern void LedsOff(void);
extern void ShowLeds(void);
extern void CheckDevices(void);   
extern void TimePlaceDigit(byte uur, byte minuut);
extern uint32_t Wheel(byte WheelPos);
extern uint32_t FuncCRGBW( uint32_t Red, uint32_t Green, uint32_t Blue, uint32_t White);
void Laatzien(void);
void Laatzien(int Delaymsec); 
void SetColours(void);
void SetSecondColour(void);
void DisplayTime(void);
void Dutch(void);
void English(void);
void German(void);
void French(void);
void DitLeHeure(void);

extern struct tm timeinfo;                                             // Storage of time used in clock for time display

                     #ifdef FOURLANGUAGECLOCK
#define NL
#define UK
#define DE
#define FR    
                     #endif //FOURLANGUAGECLOCK  

//------------------------------------------------------------------------------
// RGBW LEDs Definition of LEDs to be used for minutes.
// Normally the number start at the end of the last LED used.
// In this case 139
// These LEDs can also be used in the white LED clock. 
//------------------------------------------------------------------------------
#define MIN0   ColorLeds("M0",    NUM_LEDS-4, NUM_LEDS-1, black); 
#define MIN1   ColorLeds("M1",    NUM_LEDS-4, NUM_LEDS-4, LetterColor); 
#define MIN2   ColorLeds("M2",    NUM_LEDS-3, NUM_LEDS-3, LetterColor); 
#define MIN3   ColorLeds("M3",    NUM_LEDS-2, NUM_LEDS-2, LetterColor);
#define MIN4   ColorLeds("M4",    NUM_LEDS-1, NUM_LEDS-1, LetterColor);                                           
                                      
//------------------------------------------------------------------------------
// RGBW LEDs Definition of LEDs to be used for the time.
// Changes these numberings to your design if needed
//------------------------------------------------------------------------------
                     #ifdef NL144CLOCK
// const uint32_t NUM_LEDS  =  144 + 4;                             // How many leds in  strip? + 4 for the minutes
// const byte MATRIX_WIDTH  =  12;                                  // Grid size For digital display mode.  
// const byte MATRIX_HEIGHT =  12;                                  // Grid size For digital display mode.
const uint32_t NUM_LEDS  =  256;   // Temporary for 16x16LED WS2812. To clear all LEDs. During uploading LEDs get randomly On and Off
const byte MATRIX_WIDTH  =  16;                                    // Grid size For digital display mode.
const byte MATRIX_HEIGHT =  16;                                    // Grid size For digital display mode.

#define HET     ColorLeds("Het",     0,   2, MINColor);   
#define IS      ColorLeds("is",      4,   5, SECColor);  ColorLeds("", 8,10, 0); Is = true;
#define WAS     ColorLeds("was",     8,  10, SECColor);  ColorLeds("", 4, 5, 0); Is = false;
#define PRECIES ColorLeds("precies", 17, 23, LetterColor);
#define MTIEN   ColorLeds("tien",    12, 15, LetterColor); 
#define MVIJF   ColorLeds("vijf",    25, 28, LetterColor); 
#define KWART   ColorLeds("kwart",   30, 34, LetterColor);
#define VOOR    ColorLeds("voor",    44, 47, LetterColor);
#define OVER    ColorLeds("over",    38, 41, LetterColor);
#define HALF    ColorLeds("half",    48, 51, LetterColor);
#define MIDDER  ColorLeds("midder",  53, 58, LetterColor);
#define VIJF    ColorLeds("vijf",    66, 69, LetterColor);
#define TWEE    ColorLeds("twee",    60, 63, LetterColor);
#define EEN     ColorLeds("een",     72, 74, LetterColor);
#define VIER    ColorLeds("vier",    77, 80, LetterColor);
#define TIEN    ColorLeds("tien",    91, 94, LetterColor);
#define TWAALF  ColorLeds("twaalf",  84, 89, LetterColor);
#define DRIE    ColorLeds("drie",    96, 99, LetterColor);
#define NEGEN   ColorLeds("negen",  102,106, LetterColor);
#define ACHT    ColorLeds("acht",   114,117, LetterColor);
#define NACHT   ColorLeds("nacht",  114,118, LetterColor);
#define ZES     ColorLeds("zes",    110,112, LetterColor);
#define ZEVEN   ColorLeds("zeven",  121,125, LetterColor);
#define ELF     ColorLeds("elf",    128,130, LetterColor);
#define NOEN    ColorLeds("noen",   139,142, LetterColor);
#define UUR     ColorLeds("uur",    134,136, LetterColor);
#define EDSOFT  ColorLeds("EdSoft", 132,132, LetterColor);
#define MIN1    ColorLeds("M1",     110, 110, LetterColor); 
#define MIN2    ColorLeds("M2",     111, 111, LetterColor);                                   // MINColor); 
#define MIN3    ColorLeds("M3",     112, 112, LetterColor);
#define MIN4    ColorLeds("M4",     113, 113, LetterColor);  
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
#define NTPT    ColorLed( 96, orange);  ColorLed( 97, orange);  ColorLed( 98, orange);        // NTP
#define RTCT    ColorLed( 22, green);   ColorLed( 23, green);   ColorLed( 24, green);         // RTC
#define BLEONOFF ColorLed( 80, blue);   ColorLed( 81, blue);    ColorLed( 82, blue);          // BLE
                     #endif //NL144CLOCK


                     #ifdef NLM1M2M3M4L114
const uint32_t NUM_LEDS  =  114;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  11;                                    // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  10;                                    // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   2, MINColor);           // Special adapted clock face
#define IS      ColorLeds("is",       4,   5, SECColor);     Is = true;    //ColorLeds("", 4, 5, 0); 
#define WAS     ColorLeds("was",      4,   5, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     7,  10, LetterColor); 
#define MVOOR   ColorLeds("voor",    11,  14, LetterColor);
#define MTIEN   ColorLeds("tien",    18,  21, LetterColor); 
#define OVER    ColorLeds("over",    22,  25, LetterColor);
#define KWART   ColorLeds("kwart",   28,  32, LetterColor);
#define OVER2   ColorLeds("over",    33,  36, LetterColor);
#define HALF    ColorLeds("half",    40,  43, LetterColor);
#define VOOR    ColorLeds("voor",    44,  47, LetterColor);
#define EEN     ColorLeds("een",     51,  53, LetterColor);
#define DRIE    ColorLeds("drie",    55,  58, LetterColor);
#define TWEE    ColorLeds("twee",    62,  65, LetterColor);
#define VIER    ColorLeds("vier",    66,  69, LetterColor);
#define VIJF    ColorLeds("vijf",    70,  73, LetterColor);
#define ZES     ColorLeds("zes",     74,  76, LetterColor);
#define NEGEN   ColorLeds("negen",   77,  81, LetterColor);
#define ZEVEN   ColorLeds("zeven",   83,  87, LetterColor);
#define ACHT    ColorLeds("acht",    88,  91, LetterColor);
#define TIEN    ColorLeds("tien",    92,  95, LetterColor);
#define ELF     ColorLeds("elf",     96,  98, LetterColor);
#define TWAALF  ColorLeds("twaalf", 104, 109, LetterColor);
#define UUR     ColorLeds("uur",     99, 101, LetterColor);
#define MIN1    ColorLeds("M1",     110, 110, LetterColor); 
#define MIN2    ColorLeds("M2",     111, 111, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",     112, 112, LetterColor);
#define MIN4    ColorLeds("M4",     113, 113, LetterColor);    
                     #endif //NLM1M2M3M4L114

                                             #ifdef FOURLANGUAGECLOCK                                             
const uint32_t  NUM_LEDS = 625;                                     // How many leds in  strip?
const byte MATRIX_WIDTH  =  25;                                    // Grid size For digital display mode.
const byte MATRIX_HEIGHT =  25;                                    // Grid size For digital display mode.
                                             #endif //FOURLANGUAGECLOCK     
//------------------------------------------------------------------------------
// RGBW LEDs Definition of LEDs to be used for the time.
// in the four-lanuages clock
////------------------------------------------------------------------------------                    
                     #ifdef NL
#define HET     ColorLeds("Het",      0,   2, LetterColor);   
#define IS      ColorLeds("is",       4,   5, LetterColor); 
#define WAS     ColorLeds("was",      8,  10, LetterColor); 
#define PRECIES ColorLeds("precies", 43,  49, LetterColor);
#define MTIEN   ColorLeds("tien",    38,  41, LetterColor); 
#define MVIJF   ColorLeds("vijf",    51,  54, LetterColor); 
#define KWART   ColorLeds("kwart",   56,  60, LetterColor);
#define VOOR    ColorLeds("voor",    96,  99, LetterColor);
#define OVER    ColorLeds("over",    90,  93, LetterColor);
#define HALF    ColorLeds("half",   100, 103, LetterColor);
#define MIDDER  ColorLeds("midder", 105, 110, LetterColor);
#define VIJF    ColorLeds("vijf",   144, 147, LetterColor);
#define TWEE    ColorLeds("twee",   138, 141, LetterColor);
#define EEN     ColorLeds("een",    150, 152, LetterColor);
#define VIER    ColorLeds("vier",   155, 158, LetterColor);
#define TIEN    ColorLeds("tien",   195, 198, LetterColor);
#define TWAALF  ColorLeds("twaalf", 188, 193, LetterColor);
#define DRIE    ColorLeds("drie",   200, 203, LetterColor);
#define NEGEN   ColorLeds("negen",  206, 210, LetterColor);
#define ACHT    ColorLeds("acht",   244, 247, LetterColor);
#define NACHT   ColorLeds("nacht",  244, 248, LetterColor);
#define ZES     ColorLeds("zes",    240, 242, LetterColor);
#define ZEVEN   ColorLeds("zeven",  251, 255, LetterColor);
#define ELF     ColorLeds("elf",    258, 260, LetterColor);
#define NOEN    ColorLeds("noen",   295, 298, LetterColor);
#define UUR     ColorLeds("uur",    290, 292, LetterColor);
#define EDSOFT  ColorLeds("EdSoft", 300, 311, LetterColor);
#define X_OFF   ColorLeds("",         0,   2, 0);
#define X_ON    ColorLeds("",         0,   2, LetterColor);
                     #endif //NL
                     #ifdef UK
#define IT      ColorLeds("| It",   347, 348, UKLetterColor);   
#define ISUK    ColorLeds("is",     344, 345, UKLetterColor); 
#define WASUK   ColorLeds("was",    340, 342, UKLetterColor);
#define EXACTUK ColorLeds("exact",  351, 355, UKLetterColor);
#define HALFUK  ColorLeds("half",   357, 360, UKLetterColor); 
#define TWENTY  ColorLeds("twenty", 394, 399, UKLetterColor); 
#define MFIVE   ColorLeds("five",   388, 391, UKLetterColor);
#define QUARTER ColorLeds("quarter",400, 406, UKLetterColor);
#define MTEN    ColorLeds("ten",    409, 411, UKLetterColor);
#define PAST    ColorLeds("past",   446, 449, UKLetterColor);
#define TO      ColorLeds("to",     443, 444, UKLetterColor);
#define SIXUK   ColorLeds("six",    439, 441, UKLetterColor);
#define TWO     ColorLeds("two",    451, 453, UKLetterColor);
#define FIVE    ColorLeds("five",   457, 460, UKLetterColor);
#define TWELVE  ColorLeds("twelve", 494, 499, UKLetterColor);
#define TEN     ColorLeds("ten",    488, 490, UKLetterColor);
#define ELEVEN  ColorLeds("eleven", 500, 505, UKLetterColor);
#define FOUR    ColorLeds("four",   507, 510, UKLetterColor);
#define NINE    ColorLeds("nine",   545, 548, UKLetterColor);
#define THREE   ColorLeds("three",  538, 542, UKLetterColor);
#define EIGHT   ColorLeds("eight",  553, 557, UKLetterColor);
#define ONE     ColorLeds("one",    597, 599, UKLetterColor);
#define SEVEN   ColorLeds("seven",  589, 593, UKLetterColor);
#define OCLOCK  ColorLeds("O'clock",605, 610, UKLetterColor);
                      #endif //UK
                      #ifdef DE
#define ES      ColorLeds("\nEs",  334, 335, DELetterColor);   
#define IST     ColorLeds("ist",    330, 332, DELetterColor); 
#define WAR     ColorLeds("war",    326, 328, DELetterColor); 
#define GENAU   ColorLeds("genau",  364, 368, DELetterColor);
#define MZEHN   ColorLeds("zehn",   370, 373, DELetterColor);
#define MFUNF   ColorLeds("funf",   383, 386, DELetterColor);
#define VIERTEL ColorLeds("viertel",375, 381, DELetterColor);
#define ZWANZIG ColorLeds("zwanzig",413, 419, DELetterColor);
#define KURZ    ColorLeds("kurz",   421, 424, DELetterColor);
#define VOR     ColorLeds("vor",    433, 435, DELetterColor);
#define NACH    ColorLeds("nach",   427, 430, DELetterColor);
#define HALB    ColorLeds("halb",   465, 468, DELetterColor);
#define FUNF    ColorLeds("funf",   471, 474, DELetterColor);
#define EINS    ColorLeds("eins",   483, 486, DELetterColor);
#define VIERDE  ColorLeds("vier",   478, 481, DELetterColor);
#define ZEHN    ColorLeds("zehn",   514, 517, DELetterColor);
#define ZWOLF   ColorLeds("zwolf",  520, 524, DELetterColor);
#define DREI    ColorLeds("drei",   532, 535, DELetterColor);
#define NEUN    ColorLeds("neun",   526, 529, DELetterColor);
#define ACHTDE  ColorLeds("acht",   565, 568, DELetterColor);
#define SECHS   ColorLeds("sechs",  570, 574, DELetterColor);
#define SIEBEN  ColorLeds("sieben", 580, 585, DELetterColor);
#define ZWEI    ColorLeds("zwei",   575, 578, DELetterColor);
#define ELFDE   ColorLeds("elf",    614, 616, DELetterColor);
#define UHR     ColorLeds("uhr",    621, 623, DELetterColor);
                      #endif //DE
                      #ifdef FR
#define IL      ColorLeds("| Il",    13,  14, FRLetterColor);   
#define EST     ColorLeds("est",     16,  18, FRLetterColor); 
#define ETAIT   ColorLeds("etait",   20,  24, FRLetterColor);
#define EXACT   ColorLeds("exact",   31,  35, FRLetterColor);
#define SIX     ColorLeds("six",     27,  29, FRLetterColor); 
#define DEUX    ColorLeds("deux",    64,  67, FRLetterColor); 
#define TROIS   ColorLeds("trois",   69,  73, FRLetterColor);
#define ONZE    ColorLeds("onze",    83,  86, FRLetterColor);
#define QUATRE  ColorLeds("quatre",  75,  80, FRLetterColor);
#define MINUIT  ColorLeds("minuit", 113, 118, FRLetterColor);
#define DIX     ColorLeds("dix",    120, 122, FRLetterColor);
#define CINQ    ColorLeds("cinq",   133, 136, FRLetterColor);
#define NEUF    ColorLeds("neuf",   129, 132, FRLetterColor);
#define MIDI    ColorLeds("midi",   125, 128, FRLetterColor);
#define HUIT    ColorLeds("huit",   164, 167, FRLetterColor);
#define SEPT    ColorLeds("sept",   171, 174, FRLetterColor);
#define UNE     ColorLeds("une",    184, 186, FRLetterColor);
#define HEURE   ColorLeds("heure",  178, 182, FRLetterColor);
#define HEURES  ColorLeds("heures", 177, 182, FRLetterColor);
#define ET      ColorLeds("et",     213, 214, FRLetterColor);
#define MOINS   ColorLeds("moins",  216, 220, FRLetterColor);
#define LE      ColorLeds("le",     222, 223, FRLetterColor);
#define DEMI    ColorLeds("demie",  229, 233, FRLetterColor);
#define QUART   ColorLeds("quart",  263, 267, FRLetterColor);
#define MDIX    ColorLeds("dix",    270, 272, FRLetterColor);
#define VINGT   ColorLeds("vingt",  282, 286, FRLetterColor);
#define MCINQ   ColorLeds("cinq",   277 ,280, FRLetterColor);
#define DITLEHEURE DitLeHeure();
                      #endif //FR

//--------------------------------------------                                                //
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void DisplayTime(void)
{ 
 LedsOff();                                                                                  // Start by clearing the display to a known state   
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else                                                                                        // If not a difital display 
   {
                     #ifdef NL144CLOCK
    Dutch();                                                                                 // If TurnLEDsOff is true a new time must be send to the LEDstrip      
                     #endif  //NL144CLOCK
                     #if (defined NLM1M2M3M4L90) || (defined NLM1M2M3M4L140)
    Dutch();                                                                                 // If TurnLEDsOff is true a new time must be send to the LEDstrip      
                     #endif  //NLM1M2M3M4Lxxx
                     #ifdef FOURLANGUAGECLOCK
    SetColours();                                                                            // Set the colours for the chosen palette   
    Dutch();
    English();
    German();
    French();
                     #endif  //FOURLANGUAGECLOCK       
   }  
 ShowLeds();                                                                                  // And turn on the LEDs
}
                    #ifdef FOURLANGUAGECLOCK
//--------------------------------------------                                                //
//  CLOCK Set second for 4 language clock
//--------------------------------------------
void SetColours(void)
{  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor   = DefaultColor; 
                       UKLetterColor = UKDefaultColor;
                       DELetterColor = DEDefaultColor;
                       FRLetterColor = FRDefaultColor;                     break;             // HET IS WAS changing   
   case HOURLYCOLOUR : LetterColor   = HourColor[timeinfo.tm_hour];
                       UKLetterColor = HourColor[(timeinfo.tm_hour+1)%24];
                       DELetterColor = HourColor[(timeinfo.tm_hour+2)%24];
                       FRLetterColor = HourColor[(timeinfo.tm_hour+3)%24];  break;            // A colour every hour   
   case WHITECOLOR   : LetterColor   = white; 
                       UKLetterColor = white;
                       DELetterColor = white;
                       FRLetterColor = white;                               break;            // All white
   case OWNCOLOUR    : LetterColor   = Mem.OwnColour; 
                       UKLetterColor = Mem.OwnColour;
                       DELetterColor = Mem.OwnColour;
                       FRLetterColor = Mem.OwnColour;                       break;
   case OWNHETISCLR  : LetterColor   = Mem.OwnColour; 
                       UKLetterColor = Mem.OwnColour;
                       DELetterColor = Mem.OwnColour;
                       FRLetterColor = Mem.OwnColour;                       break;            // Own colour HET IS WAS changing  
   case WHEELCOLOR   : LetterColor   = Wheel((timeinfo.tm_min*4));       
                       UKLetterColor = Wheel((60+timeinfo.tm_min*4));
                       DELetterColor = Wheel((120+timeinfo.tm_min*4));
                       FRLetterColor = Wheel((180+timeinfo.tm_min*4));      break;            // Colour of all letters changes per second
   case DIGITAL      :                                                      break;            //LetterColor = Mem.OwnColour;                 break;            // digital display of time. No IS WAS turn color off in display
  }
}
                     #endif  //FOURLANGUAGECLOCK       

//--------------------------------------------                                                //
//  CLOCK Set second color
//  Set the colour per second of 'IS' and 'WAS'
//---------------------------------------------
void SetSecondColour(void)
{
                             #ifdef FOURLANGUAGECLOCK
  return;                                                                                     // 4-language clock uses SetColours() once per minute
                             #endif  //FOURLANGUAGECLOCK    

 MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4,0,0); 
 SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4,0,0 );   
                                                                                              // Light up  IS or WAS with the proper colour  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = DefaultColor;                                     break; // Yellow text with changing MIN and SEC  
   case HOURLYCOLOUR : LetterColor = HourColor[timeinfo.tm_hour];                      break; // A colour every hour
   case WHITECOLOR   : LetterColor = MINColor = SECColor = white;                      break; // all white
   case OWNCOLOUR    : LetterColor = Mem.OwnColour; MINColor = SECColor = LetterColor; break; // own colour
   case OWNHETISCLR  : LetterColor = Mem.OwnColour;                                    break; // own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = Wheel((timeinfo.tm_min*4)); break; // Colour of all letters changes per second
   case DIGITAL      : MINColor = SECColor = 0;                                        break; // digital display of time. No IS WAS turn color off in display
  }
 NoTextInLeds  = true;                                                                        // Flag to control printing of the text IS en WAS in serial
 if(Is) {IS;} 
 else {WAS;} 
 NoTextInLeds  = false;                                                                       // Flag to control printing of the text IS en WAS in serial
 ShowLeds();                                                                                  // Updating IS and WAS with ShowLeds is done here to avoid updating all letters every second with Displaytime function
//    Serial.print("SecColor: ");    Serial.println(SECColor,HEX);  
}

//--------------------------------------------                                                //
//  CLOCK Dutch clock display
// 
//--------------------------------------------

                      #if defined(NL) || defined(NL144CLOCK)  
void Dutch(void)
{
HET;                                                                                          // HET  is always on
 if (timeinfo.tm_hour == 12 && timeinfo.tm_min == 0 && random(2)==0) { IS; NOEN; return; }
 if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { IS; MIDDER; NACHT; return; } 
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
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
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

  switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
 {
  case  0: MIN0; break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }  

}
                                    #endif //NL144CLOCK

                     #if (defined NLM1M2M3M4L90) || (defined NLM1M2M3M4L140)
void Dutch(void)
{
HET;                                                                                          // HET  is always on
switch (timeinfo.tm_min)
 {
  case  0: 
  case  1:  
  case  2: 
  case  3: 
  case  4: IS;  break;                                                                        // HET IS .. UUR
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: IS;  MVIJF; OVER; break;                                                           // HET IS VIJF OVER .. 
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: IS;  MTIEN; OVER; break;                                                           // Et cetera
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: IS;  KWART; OVER2; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: IS;  MTIEN; MVOOR; HALF; break;
  case 25: 
  case 26: 
  case 27: 
  case 28:
  case 29: IS;  MVIJF; MVOOR; HALF; break;
  case 30:
  case 31:
  case 32:
  case 33:
  case 34: IS;  HALF; break;
  case 35: 
  case 36: 
  case 37:
  case 38:
  case 39: IS;  MVIJF; OVER; HALF; break;
  case 40: 
  case 41:
  case 42:
  case 43:
  case 44: IS;  MTIEN; OVER; HALF; break;
  case 45: 
  case 46: 
  case 47:
  case 48:
  case 49: IS;  KWART; VOOR; break;
  case 50:
  case 51:
  case 52:
  case 53:
  case 54: IS;  MTIEN; VOOR;  break;
  case 55:
  case 56: 
  case 57: 
  case 58:
  case 59: IS;  MVIJF; VOOR; break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 19 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
//  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: 
  case  4: UUR;  break; 
 }

 switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
 {
  case  0: MIN0; break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }
}
                     #endif  //NLM1M2M3M4

                     #ifdef UK
//--------------------------------------------                                                //
//  CLOCK English clock display
//--------------------------------------------
void English(void)
{
 IT;                                                                                          // IT is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { ISUK; MID; NIGHT; return; } 
 switch (timeinfo.tm_min)
 {
  case  0: ISUK;  EXACTUK; break;
  case  1: ISUK;  break;
  case  2: 
  case  3: WASUK; break;
  case  4: 
  case  5: 
  case  6: ISUK;  MFIVE; PAST; break;
  case  7: 
  case  8: WASUK; MFIVE; PAST; break;
  case  9: 
  case 10: 
  case 11: ISUK;  MTEN; PAST; break;
  case 12: 
  case 13: WASUK; MTEN; PAST; break;
  case 14: 
  case 15: 
  case 16: ISUK;  QUARTER; PAST; break;
  case 17: 
  case 18: WASUK; QUARTER; PAST; break;
  case 19: 
  case 20: 
  case 21: ISUK;  TWENTY; PAST; break;
  case 22: 
  case 23: WASUK; TWENTY; PAST; break;
  case 24: 
  case 25: 
  case 26: ISUK;  TWENTY; MFIVE; PAST; break;
  case 27: 
  case 28: WASUK; TWENTY; MFIVE; PAST; break;
  case 29: ISUK;  HALFUK; PAST; break;
  case 30: ISUK;  EXACTUK; HALFUK; PAST; break;
  case 31: ISUK;  HALFUK; PAST; break;
  case 32: 
  case 33: WASUK; HALFUK; PAST; break;
  case 34: 
  case 35: 
  case 36: ISUK;  TWENTY; MFIVE; TO; break;
  case 37: 
  case 38: WASUK; TWENTY; MFIVE; TO; break;
  case 39: 
  case 40: 
  case 41: ISUK;  TWENTY; TO; break;
  case 42: 
  case 43: WASUK; TWENTY; TO break;
  case 44: 
  case 45: 
  case 46: ISUK;  QUARTER; TO; break;
  case 47: 
  case 48: WASUK; QUARTER; TO; break;
  case 49: 
  case 50: 
  case 51: ISUK;  MTEN; TO;  break;
  case 52: 
  case 53: WASUK; MTEN; TO;  break;
  case 54: 
  case 55: 
  case 56: ISUK;  MFIVE; TO; break;
  case 57: 
  case 58: WASUK; MFIVE; TO; break;
  case 59: ISUK;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  ONE; break;
  case 14:
  case 2:  TWO; break;
  case 15:
  case 3:  THREE; break;
  case 16:
  case 4:  FOUR; break;
  case 17:
  case 5:  FIVE; break;
  case 18:
  case 6:  SIXUK; break;
  case 19:
  case 7:  SEVEN; break;
  case 20:
  case 8:  EIGHT; break;
  case 21:
  case 9:  NINE; break;
  case 22:
  case 10: TEN; break;
  case 23:
  case 11: ELEVEN; break;
  case 0:
  case 12: TWELVE; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: OCLOCK;  break; 
 }
}
                     #endif //UK
                     #ifdef DE
//--------------------------------------------                                                //
//  CLOCK German clock display
//--------------------------------------------
void German(void)
{
  ES;                                                                                         // ES is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) {IST; MITTER; NACHTDE; return; } 
 switch (timeinfo.tm_min)
 {
  case  0: IST;  GENAU; break;
  case  1: IST; KURZ; NACH; break;
  case  2: 
  case  3: WAR; break;
  case  4: 
  case  5: 
  case  6: IST; MFUNF; NACH; break;
  case  7: 
  case  8: WAR; MFUNF; NACH; break;
  case  9: 
  case 10: 
  case 11: IST; MZEHN; NACH; break;
  case 12: 
  case 13: WAR; MZEHN; NACH; break;
  case 14: 
  case 15: 
  case 16: IST; VIERTEL; NACH; break;
  case 17: 
  case 18: WAR; VIERTEL; NACH; break;
  case 19: 
  case 20: 
  case 21: IST; MZEHN; VOR; HALB; break;
  case 22: 
  case 23: WAR; MZEHN; VOR; HALB; break;
  case 24: 
  case 25: 
  case 26: IST; MFUNF; VOR; HALB; break;
  case 27: 
  case 28: WAR; MFUNF; VOR; HALB; break;
  case 29: IST; KURZ;  VOR; HALB; break;
  case 30: IST; GENAU; HALB; break;
  case 31: IST; KURZ;  NACH; HALB; break;
  case 32: 
  case 33: WAR; HALB; break;
  case 34: 
  case 35: 
  case 36: IST; MFUNF; NACH; HALB; break;
  case 37: 
  case 38: WAR; MFUNF; NACH; HALB; break;
  case 39: 
  case 40: 
  case 41: IST; MZEHN; NACH; HALB; break;
  case 42: 
  case 43: WAR; MZEHN; NACH; HALB; break;
  case 44: 
  case 45: 
  case 46: IST; VIERTEL; VOR; break;
  case 47: 
  case 48: WAR; VIERTEL; VOR; break;
  case 49: 
  case 50: 
  case 51: IST; MZEHN; VOR;  break;
  case 52: 
  case 53: WAR; MZEHN; VOR;  break;
  case 54: 
  case 55: 
  case 56: IST; MFUNF; VOR; break;
  case 57: 
  case 58: WAR; MFUNF; VOR; break;
  case 59: IST;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EINS; break;
  case 14:
  case 2: ZWEI; break;
  case 15:
  case 3: DREI; break;
  case 16:
  case 4: VIERDE; break;
  case 17:
  case 5: FUNF; break;
  case 18:
  case 6: SECHS; break;
  case 19:
  case 7: SIEBEN; break;
  case 20:
  case 8: ACHTDE; break;
  case 21:
  case 9: NEUN; break;
  case 22:
  case 10: ZEHN; break;
  case 23:
  case 11: ELFDE; break;
  case 0:
  case 12: ZWOLF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UHR;  break; 
 }
}
                     #endif //DE
                     #ifdef FR
//--------------------------------------------                                                //
//  CLOCK French clock display
//--------------------------------------------
void French(void)
{
 IL;                                                                                          // IL is always on
 switch (timeinfo.tm_min)
 {
  case  0: EST;   EXACT; DITLEHEURE; break;
  case  1: EST;   DITLEHEURE; break;
  case  2: 
  case  3: ETAIT; DITLEHEURE; break;
  case  4: 
  case  5: 
  case  6: EST;   DITLEHEURE; MCINQ; break;
  case  7: 
  case  8: ETAIT; DITLEHEURE; MCINQ; break;
  case  9: 
  case 10: 
  case 11: EST;   DITLEHEURE; MDIX;  break;
  case 12: 
  case 13: ETAIT; DITLEHEURE; MDIX;  break;
  case 14: 
  case 15: 
  case 16: EST;   DITLEHEURE; ET; QUART; break;
  case 17: 
  case 18: ETAIT; DITLEHEURE; ET; QUART; break;
  case 19: 
  case 20: 
  case 21: EST;   DITLEHEURE; VINGT; break;
  case 22: 
  case 23: ETAIT; DITLEHEURE; VINGT; break;
  case 24: 
  case 25: 
  case 26: EST;   DITLEHEURE; VINGT; MCINQ; break;
  case 27: 
  case 28: ETAIT; DITLEHEURE; VINGT; MCINQ; break;
  case 29: EST;   DITLEHEURE; ET; DEMI; break;
  case 30: EST;   EXACT; DITLEHEURE;  ET; DEMI; break;
  case 31: EST;   DITLEHEURE; ET; DEMI; break;
  case 32: 
  case 33: ETAIT; DITLEHEURE; ET; DEMI; break;
  case 34: 
  case 35: 
  case 36: EST;   DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 37: 
  case 38: ETAIT; DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 39: 
  case 40: 
  case 41: EST;   DITLEHEURE; MOINS; VINGT;  break;
  case 42: 
  case 43: ETAIT; DITLEHEURE; MOINS; VINGT;  break;
  case 44: 
  case 45: 
  case 46: EST;   DITLEHEURE; MOINS; LE; QUART; break;
  case 47: 
  case 48: ETAIT; DITLEHEURE; MOINS; LE; QUART; break;
  case 49: 
  case 50: 
  case 51: EST;   DITLEHEURE; MOINS; MDIX;   break;
  case 52: 
  case 53: ETAIT; DITLEHEURE; MOINS; MDIX;   break;
  case 54: 
  case 55: 
  case 56: EST;   DITLEHEURE; MOINS; MCINQ;  break;
  case 57: 
  case 58: ETAIT; DITLEHEURE; MOINS; MCINQ;  break;
  case 59: EST;   DITLEHEURE;  break;
 }
}

void DitLeHeure(void)
{
 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  UNE;    HEURE;  break;
  case 14:
  case 2:  DEUX;   HEURES;  break;
  case 15:
  case 3:  TROIS;  HEURES;  break;
  case 16:
  case 4:  QUATRE; HEURES; break;
  case 17:
  case 5:  CINQ;   HEURES;   break;
  case 18:
  case 6:  SIX;    HEURES;   break;
  case 19:
  case 7:  SEPT;   HEURES;  break;
  case 20:
  case 8:  HUIT;   HEURES; break;
  case 21:
  case 9:  NEUF;   HEURES; break;
  case 22:
  case 10: DIX;    HEURES; break;
  case 23:
  case 11: ONZE;   HEURES; break;
  case 0:  MINUIT; break;
  case 12: MIDI;   break;
 } 
}
                     #endif //FR

//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------
void Selftest(void)
{ 
 LedsOff(); 
 int Delayms = 900;
                      #if defined NLM1M2M3M4L114
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); MVIJF; Laatzien(Delayms); MTIEN;   Laatzien(Delayms); MVOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms);
 KWART; Laatzien(Delayms); HALF;  Laatzien(Delayms); OVER2; Laatzien(Delayms); VOOR;    Laatzien(Delayms); EEN;    Laatzien(Delayms); TWEE;   Laatzien(Delayms);  
 DRIE;  Laatzien(Delayms); VIER;  Laatzien(Delayms); VIJF;  Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms); ACHT;   Laatzien(Delayms); 
 NEGEN; Laatzien(Delayms); TIEN;  Laatzien(Delayms); ELF;   Laatzien(Delayms); TWAALF;  Laatzien(Delayms); UUR;    Laatzien(Delayms); 
 MIN1;  Laatzien(Delayms); MIN2;  Laatzien(Delayms); MIN3;  Laatzien(Delayms); MIN4;    Laatzien(Delayms);
                      #endif  //NLM1M2M3M4
                      #ifdef NL144CLOCK                    
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); WAS;    Laatzien(Delayms); PRECIES; Laatzien(Delayms); MTIEN;  Laatzien(Delayms);  MVIJF; Laatzien(Delayms);    
 KWART; Laatzien(Delayms); VOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms); HALF;    Laatzien(Delayms); MIDDER; Laatzien(Delayms);  VIJF;  Laatzien(Delayms);
 TWEE;  Laatzien(Delayms); EEN;   Laatzien(Delayms); VIER;   Laatzien(Delayms); TIEN;    Laatzien(Delayms); TWAALF; Laatzien(Delayms);  DRIE;  Laatzien(Delayms);
 NEGEN; Laatzien(Delayms); NACHT; Laatzien(Delayms); ACHT;   Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms);  ELF;   Laatzien(Delayms); 
 NOEN;  Laatzien(Delayms); UUR;   Laatzien(Delayms); EDSOFT; Laatzien(Delayms);
                      #endif  //NL144CLOCK
 Tekstprintln("*");                         
}
//--------------------------------------------                                                //
// LED Turn On and Off the LED's after 200 milliseconds
//--------------------------------------------
void Laatzien(int Delaymsec) { delay(Delaymsec); Laatzien();}
void Laatzien()
{ 
 ShowLeds();
 LedsOff(); 
 CheckDevices();                                                                              // Check for input from input devices
}
