#ifndef RGB_H
#define RGB_H

#include <FRTimer.h>
#include <FRRGBLED.h>
#include <SSD1306Ascii.h>     
#include <SSD1306AsciiWire.h> 
                              
const byte PINSWITCH = 35;    


const int OLED_ADDRESS = 0x3C; 


RGBLED myLED; 
SSD1306AsciiWire myOLED;

void OLEDSetup()
{
    myOLED.begin(&Adafruit128x32, OLED_ADDRESS);
    myOLED.setFont(Adafruit5x7); 
    myOLED.clear();             
    Message("OLED initialized", myLED, GREEN, Serial, myOLED);
    delay(1000);
}

void Message(const String &_Message, RGBLED &_LED, rgbcolors _Color, HardwareSerial &_Serial, SSD1306AsciiWire &_OLED)
{
    _LED.SetColor(_Color);
    _Serial.println(_Message);
    _OLED.println(_Message);
}



#endif