// 8 LEDs - Written by Mr.K 2021.10.16
// This code is written to make 8 LEDs light on and off one
// after another in a continuous loop using for loops and variables.
// variables, arrays, for loop

int ledPins[] = {1, 5, 2, 6, 3, 7, 4, 8};                   // array to hold pin numbers
int delayTime[] = {500, 400, 300, 200, 100, 200, 300, 400}; // delay time variable

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
    for (int k = 0; k < 8; k += 1)
    {
        // turn LEDs on then off one at a time with delayTime delay
        for (int i = 0; i <= 7; i++)
        {
            digitalWrite(ledPins[i], HIGH);
            delay(delayTime[k]);
        }

        for (int j = 8; j >= 0; j--)
        {
            digitalWrite(ledPins[j], LOW);
            delay(delayTime[k]);
        }
    }
}
