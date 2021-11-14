
/*
  DIN connects to pin 12
  CLK connects to pin 11
  CS connects to pin 10
*/

#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 2);

void setup()
{
    Serial.begin(9600);
    lc.shutdown(0, false);
    // Set brightness to a medium value
    lc.setIntensity(0, 1);
    // Clear the display
    lc.clearDisplay(0);
    lc.shutdown(1, false);
    lc.setIntensity(1, 1);
    lc.clearDisplay(1);
}

int screen[8][15] = {

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

};

void loop()
{
    for (int x = 0; x <= 64; x++)
    {
        Serial.println(x);
        drawRadius(200, x * 2.8125);
        if (x * 2.8125 == 180)
        {
            screen[7][0];
        }
        draw(screen);
        delay(100);
    }

    //  for (int q = 0; q < 15; q++) {
    //    for (int l = 0; l < 8; l++) {
    //      screen[l][q] = 1;
    //    }
    //  }
    //     draw(screen);

    delay(100000000000000000);
}

void drawRadius(int dist, float angle)
{
    float adjacent = dist * cos(angle * 1000 / 57296);
    int ledDist = abs(adjacent / 25);
    //Serial.println(adjacent);
    //Serial.println(ledDist);

    if (ledDist == 0)
    {
        for (int vert = 0; vert < dist / 25; vert++)
        {
            addtoScreen(0, vert, true, angle);
        }
    }

    for (int x = 0; x < ledDist; x++)
    {
        for (int check = 0; check < 10; check++)
        {
            float mapVal = ((x * 25) + (check * 2.5)) * (tan(angle * 1000 / 57296));
            int ledVal = map(mapVal, 0, 200, 0, 8);
            //           Serial.println(x);
            //            Serial.println(abs(ledVal));
            //            Serial.println();
            addtoScreen(x, abs(ledVal), true, angle);
        }
        //  while(true){}
    }
}
void addtoScreen(int x, int y, boolean state, float angle)
{
    int newX = 0;
    if (angle < 90)
    {
        newX = x + 7;
    }
    else
    {
        newX = abs(x - 7);
    }
    if (state == true)
    {
        screen[abs(y - 7)][newX] = 1;
    }
    else
    {
        screen[abs(y - 7)][newX] = 0;
    }
}

void draw(int screen[8][15])
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 15; x++)
        {
            if (screen[y][x] == 1)
            {
                if (x < 8)
                {
                    lc.setLed(1, x, abs(y - 7), true);
                }
                else
                {
                    lc.setLed(0, x - 8, abs(y - 7), true);
                }
            }
        }
    }
}
