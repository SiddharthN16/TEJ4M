#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 1);

int x1Val, y1Val, x2Val, y2Val, x1Map, y1Map, x2Map, y2Map;

void setup()
{
    Serial.begin(9600);

    lc.shutdown(0, false);
    lc.setIntensity(0, 12);
    lc.clearDisplay(0);
}

void loop()
{
    // x1Map = map(x1Val, 0, 510, 0, 8);
    // y1Map = map(y1Val, 0, 510, 7, 0);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            lc.setLed(0, i, j, true);
            delay(100);
        }
    }

    lc.setLed(0, 0, 0, true);
    lc.setLed(0, 7, 7, true);
    lc.clearDisplay(0);
}