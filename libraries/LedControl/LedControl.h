/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControl_h
#define LedControl_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
const static byte charTable[128] = {
    0b01111110, 0b00110000, 0b01101101, 0b01111001,
    0b00110011, 0b01011011, 0b01011111, 0b01110000,
    0b01111111, 0b01111011, 0b01110111, 0b00011111,
    0b00001101, 0b00111101, 0b01001111, 0b01000111,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b10000000, 0b00000001, 0b10000000, 0b00000000,
    0b01111110, 0b00110000, 0b01101101, 0b01111001,
    0b00110011, 0b01011011, 0b01011111, 0b01110000,
    0b01111111, 0b01111011, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b01110111, 0b00011111, 0b00001101,
    0b00111101, 0b01001111, 0b01000111, 0b00000000,
    0b00110111, 0b00000000, 0b00000000, 0b00000000,
    0b00001110, 0b00000000, 0b00000000, 0b00000000,
    0b01100111, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00001000,
    0b00000000, 0b01110111, 0b00011111, 0b00001101,
    0b00111101, 0b01001111, 0b01000111, 0b00000000,
    0b00110111, 0b00000000, 0b00000000, 0b00000000,
    0b00001110, 0b00000000, 0b00000000, 0b00000000,
    0b01100111, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000
};

class LedControl {
    private :
        // The array for shifting the data to the devices.
        byte spidata[16];

        // Send out a single command to the device.
        void spiTransfer(int addr, byte opcode, byte data);

        // We keep track of the LED-status for all 8 devices in this array.
        byte status[64];

        // Data is shifted out of this pin.
        int SPI_MOSI;

        // The clock is signaled on this pin.
        int SPI_CLK;

        // This one is driven LOW for chip selection.
        int SPI_CS;

        // The maximum number of devices we use.
        int maxDevices;

   public:
      /**
       * \brief Create a new controler
       *
       * \param dataPin     Pin on the Arduino where data gets shifted out.
       * \param clockPin    Pin for the clock.
       * \param csPin       Pin for selecting the device.
       * \param numDevices  Maximum number of devices that can be controlled.
       */
      LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);

      /**
       * \brief Gets the number of devices attached to this LedControl.
       *
       * \returns The number of devices on this LedControl.
       */
      int getDeviceCount();

      /**
       * \brief Set the shutdown (power saving) mode for the device.
       *
       * \param addr    The address of the display to control.
       * \param status  If true the device goes into power-down mode. Set to
       *                false for normal operation.
       */
      void shutdown(int addr, bool status);

      /**
       * \brief Set the number of digits (or rows) to be displayed.
       *
       * \note See datasheet for side effects of the scan limit on the
       *       brightness of the display.
       *
       * \param addr   Address of the display to control.
       * \param limit  Number of digits to be displayed (1..8).
       */
      void setScanLimit(int addr, int limit);

      /**
       * \brief Set the brightness of the display.
       *
       * \params addr       The address of the display to control.
       * \params intensity  The brightness of the display (0..15).
       */
      void setIntensity(int addr, int intensity);

      /**
       * \brief Switch all LEDs on the display off.
       *
       * \param addr  Address of the display to control
       */
      void clearDisplay(int addr);

      /**
       * \brief Set the status of a single LED.
       *
       * \param addr   Address of the display.
       * \param row    The row of the LED (0..7).
       * \param col    The column of the LED (0..7).
       * \param state  If true the LED is switched on, if false it is switched
       *               off.
       */
      void setLed(int addr, int row, int col, boolean state);

      /**
       * \brief Set all 8 LEDs in a row to a new state.
       *
       * \param addr   Address of the display
       * \param row    Row which is to be set (0..7)
       * \param value  Each bit set to 1 will light up the corresponding LED.
       */
      void setRow(int addr, int row, byte value);

      /**
       * \brief Set all 8 LEDs in a column to a new state.
       *
       * \param addr   Address of the display.
       * \param col    Column which is to be set (0..7).
       * \param value  Each bit set to 1 will light up the corresponding LED.
       */
      void setColumn(int addr, int col, byte value);

      /**
       * \brief Display a hexadecimal digit on a 7-Segment Display.
       *
       * \brief addr   Address of the display.
       * \brief digit  The position of the digit on the display (0..7).
       * \brief value  The value to be displayed. (0x00..0x0F).
       * \brief dp     Sets the decimal point.
       */
      void setDigit(int addr, int digit, byte value, boolean dp);

      /**
       * \brief Display a character on a 7-Segment display.
       *
       * \note There are only a few characters that make sense here :
       *       '0','1','2','3','4','5','6','7','8','9','0',
       *       'A','b','c','d','E','F','H','L','P',
       *       '.','-','_',' '
       *
       * \param addr   Address of the display.
       * \param digit  The position of the character on the display (0..7).
       * \param value  The character to be displayed.
       * \param dp     Sets the decimal point.
       */
      void setChar(int addr, int digit, char value, boolean dp);
};

#endif // LedControl.h
