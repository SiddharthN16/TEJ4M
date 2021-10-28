/*
-----------------------------------------------------------------
Name: 8 Leds - Written by Mr.K 2021.10.16
Purpose: To make 8 LEDS light on and off one after another
in a continuous loop using for loops and variables.

Author: Siddharth Nema
Created: 2021.10.27
Updated: 2021.10.27
-----------------------------------------------------------------
*/

int ledPins[] = {1, 5, 2, 6, 3, 7, 4, 8};                    // array to hold pin numbers
int delayTimes[] = {500, 400, 300, 200, 100, 200, 300, 400}; // array of the delay times required

void setup()
{
    // set all LED pins as outputs.
    for (int i = 0; i <= 7; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop()
{
    // loop through all the delay times in delayTimess array
    for (int k = 0; k <= 7; k++)
    {
        // turn LEDs on, one at a time with delay of delayTimes[k]
        for (int i = 0; i <= 7; i++)
        {
            digitalWrite(ledPins[i], HIGH);
            delay(delayTimes[k]);
        }

        // turn LEDs off, one at a time with delay of delayTimes[k]
        for (int j = 8; j >= 0; j--)
        {
            digitalWrite(ledPins[j], LOW);
            delay(delayTimes[k]);
        }
    }
}