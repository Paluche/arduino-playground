#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define DISABLE_LED

#define I2C_ADDR      (9)

#define BUTTON_PIN    (2) // Digital IO pin connected to the button. This will
                          // be driven with a pull-up resistor so the switch
                          // should pull the pin to ground momentarily. On a
                          // high -> low transition the button press logic will
                          // execute.

#define PIXEL_PIN     (6) // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT   (150)
#define COLOR_PER_LED (3)

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT,
                                                   PIXEL_PIN,
                                                   NEO_GRB + NEO_KHZ800);



/*
 * Rainbow context
 */
static uint16_t step = 0;


/*
 * Ruban context.
 */
static uint8_t color_idx = 0;

// White over rainbow
#define WHITE_SPEED  (50)
#define WHITE_LENGTH (10)

static unsigned long lastTime = 0;
static uint16_t      head     = WHITE_LENGTH - 1;
static uint16_t      tail     = 0;

/*
 * Main context
 */
#ifndef DISABLE_LED
static uint8_t led_cnt   = 0;
#endif
static uint8_t animation = 1;
static uint8_t speed     = 0;


/*
 * Utils
 */
#if COLOR_PER_LED == 3
static uint32_t colors[] = {0,
                            strip.Color(  0,   0, 255),  // 1 Blue
                            strip.Color(  0, 255,   0),  // 2
                            strip.Color(255,   0,   0),  // 3
                            strip.Color(255,  32,   0),  // 4
                            strip.Color(255,   0, 128),  // 5
                            strip.Color(255, 128, 128),  // 6
                            strip.Color(255, 128,   0),  // 7
                            strip.Color(  0, 255, 128),  // 8
                            strip.Color(128, 255, 128),  // 9
                            strip.Color(128, 255,   0),  // 10
                            strip.Color(255,   0, 255),  // 11
                            strip.Color(128, 128, 255),  // 12
                            strip.Color(128,   0, 255),  // 13
                            strip.Color(  0, 255, 255),  // 14
                            strip.Color(255, 255, 255)}; // 15  White
#else
#error Unsupported
#endif

const char *index_color_to_str(uint8_t index)
{
    switch (index)
    {
        case 0:  return "0 OFF";
        case 1:  return "1 BLUE";
        case 2:  return "2 GREEN";
        case 3:  return "3 RED";
        case 4:  return "4 DARK ORANGE";
        case 5:  return "5 PINK";
        case 6:  return "6 LIGHT PINK";
        case 7:  return "7 ORANGE";
        case 8:  return "8 CYAN";
        case 9:  return "9 WHITE - GREEN";
        case 10: return "10 GREEN YELLOW";
        case 11: return "11 ?";
        case 12: return "12 WHITE - PURPLE";
        case 13: return "13 PURPLE";
        case 14: return "14 ?";
        case 15: return "15 WHITE";
    }

    return "WTF";
}

static void assert_speed(void)
{
    if ((animation == 1) && ((speed % 8) == 7))
    {
        // Too fast for this animation
        speed++;
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
static uint32_t wheel(byte position)
{
    position = 255 - position;

    if (position < 85)
    {
        return strip.Color(255 - position * 3, 0, position * 3, 0);
    }

    if (position < 170)
    {
        position -= 85;
        return strip.Color(0, position * 3, 255 - position * 3, 0);
    }

    position -= 170;

    return strip.Color(position * 3, 255 - position * 3, 0, 0);
}

/*
 * Train animation
 */
#define COLORS_NB (sizeof(colors) / sizeof(colors[0]))


static bool     oldState = HIGH;
static uint32_t pixels[PIXEL_COUNT];
static int      veto;
#define VETO_NB 3


static void loop_train(void)
{
    // One chance out of four to add "train" in the animation.
    uint8_t prev;

    if (veto)
    {
        veto--;
        prev = 0;
    }
    else
    {
        if (random(0, 10) == 0)
        {
            veto = VETO_NB;
            prev = random(1, COLORS_NB);
        }
    }

    for (int i = 0; i < PIXEL_COUNT; i++)
    {
        uint8_t current = pixels[i];

        pixels[i] = prev;

        if (prev != current)
        {
            strip.setPixelColor(i, colors[prev]);
        }

        prev = current;
    }
}


/*
 * Color swipe animation
 */
static void change_color_idx(void)
{
    uint8_t prev_color_idx = color_idx;

    while (prev_color_idx == color_idx)
    {
        color_idx = random(1, COLORS_NB);
    }
}

static void loop_color_swipe(void)
{
    strip.setPixelColor(step, colors[color_idx]);

    step++;

    if (step == strip.numPixels())
    {
        step = 0;
        change_color_idx();
    }
}

/*
 * Starts animation
 */
static void loop_stars(uint32_t color)
{
    // Go through the led strip and decrement the color for each pixel.
    uint16_t i;
    uint8_t *strip_pixels = strip.getPixels();

    for (i = 0;
         i < strip.numPixels() * (strip.isThreeBytesPerPixel() ? 3 : 4);
         i++)
    {
#define INCREMENT 2
        if (strip_pixels[i] > INCREMENT)
        {
            strip_pixels[i] -= INCREMENT;
        }
        else if (strip_pixels[i])
        {
            strip_pixels[i] = 0;
        }
    }

    // Create a new stars randomly. One per turn.
    uint32_t new_star = random(0, strip.numPixels());

    // Create a new star only if it's not one already.
    if (strip.getPixelColor(new_star) == 0)
    {
#if 0
        uint8_t color = random(0, COLOR_PER_LED);

        strip.setPixelColor(new_star,
                            (color == 0) ? 0xff : 0x00,
                            (color == 1) ? 0xff : 0x00,
                            (color == 2) ? 0xff : 0x00);
#else
        strip.setPixelColor(new_star, color);
#endif
    }
}

/*
 * Ruban animation
 */
static void build_ruban(void)
{
    uint16_t i;
    uint16_t j = 0;

    // A "ruban" is composed of RUBAN_FADE_SIZE pixels fading in,
    // RUBAN_STILL_SIZE pixels at maximum value and finally RUBAN_FADE_SIZE pixels fading out.
    // Index step list the position of the "ruban".
    // step goes from  ((RUBAN_FADE_SIZE * 2) + RUBAN_STILL_SIZE - 1)) to
    // strip.numPixels() + (2 * ((RUBAN_FADE_SIZE * 2) + RUBAN_STILL_SIZE - 1))
#define RUBAN_FADE_SIZE  (25)
#define RUBAN_STILL_SIZE (30)
#define RUBAN_SIZE       ((2 * RUBAN_FADE_SIZE) + RUBAN_STILL_SIZE)

    // Build the ruban, in pixels buffer.
    // Fade-in
    for (i = 1; i <= RUBAN_FADE_SIZE; i++)
    {
        pixels[j++] = strip.applyBrigthness(colors[color_idx],
                                            i,
                                            RUBAN_FADE_SIZE);
    }

    // Max pixels
    for (i = 0; i < RUBAN_STILL_SIZE; i++)
    {
        pixels[j++] = colors[color_idx];
    }

    // Fade-out
    for (i = 0; i < RUBAN_FADE_SIZE; i++)
    {
        pixels[j++] = strip.applyBrigthness(colors[color_idx],
                                            RUBAN_FADE_SIZE - i,
                                            RUBAN_FADE_SIZE);
    }
}

static void loop_ruban(void)
{
    uint16_t i;

    // Now set the colors:
#if 1
    for (i = 0; i < strip.numPixels(); i++)
    {
        if (((i + RUBAN_SIZE) >= step) && (i < step))
        {
            strip.setPixelColor(i, pixels[(i + RUBAN_SIZE) - step]);
        }
        else
        {
            strip.setPixelColor(i, 0);
        }
    }
#else
    for (i = 0; i < RUBAN_SIZE; i++)
    {
        strip.setPixelColor(i, pixels[i]);
    }
#endif

    // Increment step
    if (step == (strip.numPixels() + RUBAN_SIZE))
    {
        step = 0;
        change_color_idx();
        build_ruban();
    }
    else
    {
        step++;
    }
}

/*
 * White over rainbow animation
 */
void loop_white_over_rainbow(void)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        if ((i == tail) || (i == head))
        {
            strip.setPixelColor(i, strip.Color(128, 128, 128, 128));
        }
        else if (((tail < i) && (i < head)) ||
                 ((tail > head) && ((i > tail) || (i < head))))
        {
            strip.setPixelColor(i, strip.Color(255, 255, 255, 255));
        }
        else
        {
            strip.setPixelColor(
                i,
                wheel(((i * 256 / strip.numPixels()) + step) & 255)
            );
        }
    }

    if (millis() - lastTime > WHITE_SPEED)
    {
        head++;
        tail++;

        lastTime = millis();
    }

    head %= strip.numPixels();
    tail %= strip.numPixels();

    step++;
}

/*
 * Rainbow animation
 */
static void loop_rainbow(void)
{
    uint16_t i;

    for (i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, wheel((i + step) & 255));
    }

    step++;
}

/*
 * Mono color animation
 */
static void loop_mono_color(uint32_t color)
{
    uint16_t i;

    for (i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, color);
    }
}

/*
 * Main
 */
static void reset_context(void)
{
    memset(pixels, 0, sizeof(pixels));
    pixels[0] = 3;
    pixels[5] = 5;
    strip.clear();
    //strip.show();

    // Train context
    veto = 0;
    step = 0;

    change_color_idx();
}

// nRF52 command reception
static void receiveEvent(int bytes)
{
    (void) bytes;

    while (Wire.available() > 0)
    {
        char b = Wire.read();

        if (b == '<')
        {
            uint8_t id    = Wire.read();
            uint8_t value = Wire.read();
            char    end   = Wire.read();

            Serial.print("Received <");
            Serial.print(id);
            Serial.print(", ");
            Serial.print(value);
            Serial.print(">\r\n");

            if (end != '>')
            {
                Serial.print("Unexpected end character ");
                Serial.print(end);
                Serial.print("\r\n");
                continue;
            }

            if (id > 15)
            {
                Serial.println("Invalid id");
                continue;
            }

            if (value > 6)
            {
                Serial.println("Invalid value");
                continue;
            }

            uint8_t new_animation = animation;

            // Have value starts from 0.
            value--;

            switch (id)
            {
                // Cube 1
                case 1:
                    new_animation -= (new_animation % 6);
                    new_animation += value;
                    break;

                // Cube 2
                case 6:
                    new_animation = (value * 6) + (new_animation % 6);
                    break;

                // Cube 3
                case 11:
                    if (value != speed)
                    {
                        speed = value;
                        assert_speed();
                    }
                    break;

                default:
                    // NO-OP
                    break;
            }

            if (new_animation != animation)
            {
                reset_context();
                Serial.println(animation);
                animation = new_animation;
                Serial.println(animation);
                assert_speed();

                if (animation == 3)
                {
                    build_ruban();
                }

                if ((animation >= 6) && (animation <= 20))
                {
                    Serial.println(index_color_to_str(animation - 5));
                }
            }
        }
    }
}

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    strip.begin();
    strip.setBrightness(100);
    strip.show(); // Initialize all pixels to 'off'

    Serial.begin(115200);
    Serial.println("setup");

    // Start the I2C Bus as Slave
    Wire.begin(I2C_ADDR);

    // Attach a function to trigger when something is received.
    Wire.onReceive(receiveEvent);
}

void loop(void)
{
    // Check button for reset.
    bool newState = digitalRead(BUTTON_PIN);

    if ((newState == LOW) && (oldState == HIGH))
    {
        reset_context();
    }

    oldState = newState;

    switch (animation)
    {
        case 0:
            loop_train();
            delay(20);
            break;

        case 1:
            loop_color_swipe();
            break;

        //case 2:
        //    loop_stars(colors[14]);
        //    break;

        case 3:
            loop_ruban();
            break;

        case 4:
            loop_white_over_rainbow();
            break;

        case 5:
            loop_rainbow();
            break;

        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            loop_mono_color(colors[animation - 5]);
            break;

        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
            loop_stars(colors[animation - 5]);
            break;

        default:
            loop_mono_color(0);  // OFF
            break;
    }

    switch (speed % 8)
    {
        default:
            // No delay
            break;

        case 5:
            delay(10);
            break;

        case 4:
            delay(20);
            break;

        case 3:
            delay(30);
            break;

        case 2:
            delay(50);
            break;

        case 1:
            delay(100);
            break;

        case 0:
            delay(150);
            break;
    }

#define LED_PERIOD 16

#ifndef DISABLE_LED
    digitalWrite(LED_BUILTIN,
                 ((led_cnt++ % LED_PERIOD) < (LED_PERIOD / 2)) ? HIGH : LOW);
#endif // DISABLE_LED

    strip.show();
}
