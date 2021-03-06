// This is a demonstration on how to use an input device to trigger changes on
// your neo pixels. You should wire a momentary push button to connect from
// ground to a digital IO pin.  When you press the button it will change to a
// new pixel animation.  Note that you need to press the button once to start
// the first animation!

#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2    // Digital IO pin connected to the button. This will
                          // be driven with a pull-up resistor so the switch
                          // should pull the pin to ground momentarily. On a i
                          // high -> low transition the button press logic will execute.

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 155

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT,
                                            PIXEL_PIN,
                                            NEO_GRBW + NEO_KHZ800);

bool oldState = HIGH;
int  showType = 0;

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
    // Get current button state.
    bool newState = digitalRead(BUTTON_PIN);

    // Check if state changed from high to low (button press).
    if ((newState == LOW) && (oldState == HIGH))
    {
        // Short delay to debounce button.
        delay(20);

        // Check if button is still low after debounce.
        newState = digitalRead(BUTTON_PIN);

        if (newState == LOW)
        {
            showType++;

            if (showType > 9)
            {
                showType = 0;
            }

            startShow(showType);
        }
    }

    // Set the last button state to the old state.
    oldState = newState;
}

void startShow(int i)
{
    switch (i)
    {
        case 0:
            // Black / off
            colorWipe(strip.Color(0, 0, 0), 50);
            break;

        case 1:
            // Red
            colorWipe(strip.Color(255, 0, 0), 50);
            break;

        case 2:
            // Green
            colorWipe(strip.Color(0, 255, 0), 50);
            break;

        case 3:
            // Blue
            colorWipe(strip.Color(0, 0, 255), 50);
            break;

        case 4:
            // White
            theaterChase(strip.Color(127, 127, 127), 50);
            break;

        case 5:
            // Red
            theaterChase(strip.Color(127, 0, 0), 50);
            break;

        case 6:
            // Blue
            theaterChase(strip.Color(0, 0, 127), 50);
            break;

        case 7:
            rainbow(20);
            break;

        case 8:
            rainbowCycle(20);
            break;

        case 9:
            theaterChaseRainbow(50);
            break;
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait)
{
    uint16_t i;
    uint16_t j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < strip.numPixels(); i++)
        {
           strip.setPixelColor(i, Wheel((i + j) & 0xff));
        }

        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout.
void rainbowCycle(uint8_t wait)
{
    uint16_t i;
    uint16_t j;

    for (j = 0; j < 256*5; j++)
    {
        // 5 cycles of all colors on wheel.

        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }

        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
    for (int j = 0; j < 10; j++)
    {
        // Do 10 cycles of chasing.
        for (int q = 0; q < 3; q++)
        {
            // Turn every third pixel on.
            for (int i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, c);
            }

            strip.show();

            delay(wait);

            // Turn every third pixel off.
            for (int i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0);
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
    for (int j = 0; j < 256; j++)
    {
        // Cycle all 256 colors in the wheel.
        for (int q = 0; q < 3; q++)
        {
            // Turn every third pixel on.
            for (int i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, Wheel((i + j) % 255));
            }

            strip.show();

            delay(wait);

            // Turn every third pixel off.
            for (int i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0);
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;

    if (WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }

    if (WheelPos < 170)
    {
        WheelPos -= 85;

        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }

    WheelPos -= 170;

    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
