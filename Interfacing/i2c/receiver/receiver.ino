#include <LedControl.h>

LedControl lc = LedControl(DIN, CLK, CS, 2);

void setup()
{
    //Setup for first display
    lc.shutdown(0, false);
    lc.setIntensity(0, 12);
    lc.clearDisplay(0);

    //Setup for second display
    lc.shutdown(1, false);
    lc.setIntensity(1, 12);
    lc.clearDisplay(1);
}

void loop()
{
    lc.setLed(0, x, y, true);

    //Lighting all LEDs on row 5 of second display
    lc.setRow(1, 5, B11111111);
}