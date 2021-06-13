#include <TimerOne.h>

// display 1234
// Set cathode interface
static const int a = 1;
static const int b = 2;
static const int c = 3;
static const int d = 4;
static const int e = 5;
static const int f = 6;
static const int g = 7;
static const int dp = 8;

// Set anode interface
static const int d4 = 9;
static const int d3 = 10;
static const int d2 = 11;
static const int d1 = 12;

static uint32_t number = 0;

void WeiXuan(uint8_t n)
{
    switch (n)
    {
        case 0:
            digitalWrite(d1, LOW);
            digitalWrite(d2, LOW);
            digitalWrite(d3, LOW);
            digitalWrite(d4, HIGH);
            break;

        case 1:
            digitalWrite(d1,LOW);
            digitalWrite(d2, LOW);
            digitalWrite(d3, HIGH);
            digitalWrite(d4, LOW);
            break;

        case 2:
            digitalWrite(d1, LOW);
            digitalWrite(d2, HIGH);
            digitalWrite(d3, LOW);
            digitalWrite(d4, LOW);
            break;

        case 3:
            digitalWrite(d1,HIGH);
            digitalWrite(d2, LOW);
            digitalWrite(d3, LOW);
            digitalWrite(d4, LOW);
            break;

        default :
            digitalWrite(d1, LOW);
            digitalWrite(d2, LOW);
            digitalWrite(d3, LOW);
            digitalWrite(d4, LOW);
            break;
    }
}

void Num_0()
{
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, HIGH);
    digitalWrite(dp,HIGH);
}

void Num_1()
{
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
    digitalWrite(dp,HIGH);
}

void Num_2()
{
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, HIGH);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, HIGH);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Num_3()
{
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Num_4()
{
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Num_5()
{
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Num_6()
{
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Num_7()
{
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
    digitalWrite(dp,HIGH);
}

void Num_8()
{
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Num_9()
{
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dp,HIGH);
}

void Clear()  // Clear the screen
{
    digitalWrite(a, HIGH);
    digitalWrite(b, HIGH);
    digitalWrite(c, HIGH);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
    digitalWrite(dp,HIGH);
}

void pickNumber(uint8_t n)//Choose the number of
{
    switch(n)
    {
        case 0:
            Num_0();
            break;

        case 1:
            Num_1();
            break;

        case 2:
            Num_2();
            break;

        case 3:
            Num_3();
            break;

        case 4:
            Num_4();
            break;

        case 5:
            Num_5();
            break;

        case 6:
            Num_6();
            break;

        case 7:
            Num_7();
            break;

        case 8:
            Num_8();
            break;

        case 9:
            Num_9();
            break;

        default:
            Clear();
            break;
    }
}

//Show that x is the coordinate, Number is the number
void Display(uint8_t x, uint8_t Number)
{
    WeiXuan(x);
    pickNumber(Number);
    delay(1);
    Clear() ; //Vanishing
}

void timerIsr(void)
{
    number++;
}

void setup(void)
{
    pinMode(d1, OUTPUT);
    pinMode(d2, OUTPUT);
    pinMode(d3, OUTPUT);
    pinMode(d4, OUTPUT);
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);
    pinMode(e, OUTPUT);
    pinMode(f, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(dp, OUTPUT);

    Timer1.initialize(1000000/6);
    Timer1.attachInterrupt(timerIsr);
}

void loop()
{
    Display(0, number % 10);
    Display(1, (number / 10) % 10);
    Display(2, (number / 100) % 10);
    Display(3, (number / 1000) % 10);
}
