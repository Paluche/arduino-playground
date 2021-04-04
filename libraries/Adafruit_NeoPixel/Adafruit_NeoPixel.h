/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

// The order of primary colors in the NeoPixel data stream can vary
// among device types, manufacturers and even different revisions of
// the same item.  The third parameter to the Adafruit_NeoPixel
// constructor encodes the per-pixel byte offsets of the red, green
// and blue primaries (plus white, if present) in the data stream --
// the following #defines provide an easier-to-use named version for
// each permutation.  e.g. NEO_GRB indicates a NeoPixel-compatible
// device expecting three bytes per pixel, with the first byte
// containing the green value, second containing red and third
// containing blue.  The in-memory representation of a chain of
// NeoPixels is the same as the data-stream order; no re-ordering of
// bytes is required when issuing data to the chain.

// Bits 5,4 of this value are the offset (0-3) from the first byte of
// a pixel to the location of the red color byte.  Bits 3,2 are the
// green offset and 1,0 are the blue offset.  If it is an RGBW-type
// device (supporting a white primary in addition to R,G,B), bits 7,6
// are the offset to the white byte...otherwise, bits 7,6 are set to
// the same value as 5,4 (red) to indicate an RGB (not RGBW) device.
// i.e. binary representation:
// 0bWWRRGGBB for RGBW devices
// 0bRRRRGGBB for RGB

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W          R          G          B   // HEX representation
#define NEO_RGB  ((0 << 6) | (0 << 4) | (1 << 2) | (2)) // 0x06
#define NEO_RBG  ((0 << 6) | (0 << 4) | (2 << 2) | (1)) // 0x09
#define NEO_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2)) // 0x52
#define NEO_GBR  ((2 << 6) | (2 << 4) | (0 << 2) | (1)) // 0xA1
#define NEO_BRG  ((1 << 6) | (1 << 4) | (2 << 2) | (0)) // 0x58
#define NEO_BGR  ((2 << 6) | (2 << 4) | (1 << 2) | (0)) // 0xA4

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B   // HEX representation
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3)) // 0x1B
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2)) // 0x1E
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3)) // 0x27
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2)) // 0x36
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1)) // 0x2D
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1)) // 0x39

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3)) // 0x4B
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2)) // 0x4E
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3)) // 0x87
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2)) // 0xC6
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1)) // 0x8D
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1)) // 0xC9

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3)) // 0x63
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2)) // 0x72
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3)) // 0x93
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2)) // 0xD2
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1)) // 0xB1
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1)) // 0xE1

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0)) // 0x6C
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0)) // 0x78
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0)) // 0x9C
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0)) // 0xD8
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0)) // 0xB4
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0)) // 0xE4

// Add NEO_KHZ400 to the color order value to indicate a 400 KHz
// device.  All but the earliest v1 NeoPixels expect an 800 KHz data
// stream, this is the default if unspecified.  Because flash space
// is very limited on ATtiny devices (e.g. Trinket, Gemma), v1
// NeoPixels aren't handled by default on those chips, though it can
// be enabled by removing the ifndef/endif below -- but code will be
// bigger.  Conversely, can disable the NEO_KHZ400 line on other MCUs
// to remove v1 support and save a little space.

#define NEO_KHZ800 0x0000 // 800 KHz datastream
#ifndef __AVR_ATtiny85__
#define NEO_KHZ400 0x0100 // 400 KHz datastream
#endif

// If 400 KHz support is enabled, the third parameter to the constructor
// requires a 16-bit value (in order to select 400 vs 800 KHz speed).
// If only 800 KHz is enabled (as is default on ATtiny), an 8-bit value
// is sufficient to encode pixel color order, saving some space.

#ifdef NEO_KHZ400
typedef uint16_t neoPixelType;
#else
typedef uint8_t  neoPixelType;
#endif

class Adafruit_NeoPixel
{
    public:
        // Constructor: number of LEDs, pin number, LED type
        Adafruit_NeoPixel(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800);
        Adafruit_NeoPixel(void);
        ~Adafruit_NeoPixel();

        void begin(void);

        void show(void);

        void setPin(uint8_t p);

        void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

        void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b,
                           uint8_t w);

        void setPixelColor(uint16_t n, uint32_t c);

        void fill(uint32_t c=0, uint16_t first=0, uint16_t count=0);

        void setBrightness(uint8_t);

        void clear();

        void updateLength(uint16_t n);

        void updateType(neoPixelType t);

        uint8_t *getPixels(void) const;

        uint8_t getBrightness(void) const;

        uint8_t sine8(uint8_t) const;

        uint8_t gamma8(uint8_t) const;

        int8_t getPin(void) { return pin; };
        uint16_t numPixels(void) const;
        static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
        static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
        static uint32_t applyBrigthness(uint32_t rgbw,
                                        uint16_t level,
                                        uint16_t level_max);
        uint32_t getPixelColor(uint16_t n) const;

        inline bool canShow(void)
        {
            return (micros() - endTime) >= 300L;
        }

        /**
         * @brief Find out if each pixel is represented with 3 or 4 bytes.
         *
         * @return True if each pixel is represent with 3 bytes, false if
         *         represented with 4 bytes.
         */
        inline bool isThreeBytesPerPixel(void)
        {
            return (wOffset == rOffset);
        }

    protected:
#ifdef NEO_KHZ400            // If 400 KHz NeoPixel support enabled...
        boolean  is800KHz;   // ...true if 800 KHz pixels.
#endif
        boolean  begun;      // True if begin() previously called.
        uint16_t numLEDs;    // Number of RGB LEDs in strip.
        uint16_t numBytes;   // Size of 'pixels' buffer below (3 or 4
                             // bytes/pixel see the variable
                             // threeBytesPerPixel).
        int8_t   pin;        // Output pin number (-1 if not yet set).
        uint8_t  brightness; // Current brightness level of the strip.
        uint8_t *pixels;     // Holds LED color values (3 or 4 bytes each)
        uint8_t  rOffset;    // Index of red byte within each 3- or 4-byte pixel
        uint8_t  gOffset;    // Index of green byte.
        uint8_t  bOffset;    // Index of blue byte.
        uint8_t  wOffset;    // Index of white byte (same as rOffset if
                             // no white).
        uint32_t endTime;    // Latch timing reference.
        boolean  changes;    // Boolean to find out if there was actually any change to the strip. Don't send the command if no change.
#ifdef __AVR__
        volatile uint8_t *port;    // Output PORT register
        uint8_t           pinMask; // Output PORT bitmask
#endif
};

#endif // ADAFRUIT_NEOPIXEL_H
