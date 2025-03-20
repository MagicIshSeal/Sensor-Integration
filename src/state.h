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

void Message(const String &_Message, RGBLED &_LED, rgbcolors _Color, HardwareSerial &_Serial, SSD1306AsciiWire &_OLED)
{
    _OLED.clear();
    _LED.SetColor(_Color);
    _Serial.println(_Message);
    _OLED.println(_Message);
}

void OLEDSetup()
{
    myOLED.begin(&Adafruit128x32, OLED_ADDRESS); // Initialize the OLED
    myOLED.setFont(Adafruit5x7);                 // Set the font
    myOLED.clear();                              // Clear the display

    const int screenWidth = 128;                  // Screen width in pixels
    const int charWidth = 6;                      // Width of each character in pixels (Adafruit5x7 font)
    const int barWidth = screenWidth / charWidth; // Number of segments in the loading bar per row
    const int totalRows = 4;                      // Total rows for a 128x32 OLED

    for (int row = 0; row < totalRows; row++) // Start from row 1 (row 0 is for the message)
    {
        myOLED.setCursor(0, row); // Move to the current row
        for (int i = 0; i <= barWidth; i++)
        {
            myOLED.print("="); // Add one segment to the loading bar
            delay(50);         // Simulate loading time (adjust as needed)
        }
    }

    delay(1000); // Pause for a moment after the loading bar completes
}

#endif