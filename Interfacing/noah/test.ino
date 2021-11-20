#include <Stepper.h>
#include "LedControl.h"

const int DIN = 4;
const int CLK = 3;
const int CS = 2;
const int numOfDisplays = 2;

LedControl lc = LedControl(DIN, CLK, CS, numOfDisplays);

const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
float degree = 2.8125; // Degree per step of motor

double stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // Pin inversion to make the library work

void setup()
{
    //LCD
    Serial.begin(9600);
    lc.shutdown(0, false);
    lc.shutdown(1, false);
    lc.setIntensity(0, 1);
    lc.setIntensity(1, 1);
    lc.clearDisplay(0);
    lc.clearDisplay(1);

    //MOTOR + Sensor
    myStepper.setSpeed(2);
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int radarScreen[8][15] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int detection[8][15] = {
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
    long duration, cm;

    //scan 1 animation
    for (int x = 0; x <= 15; x++)
    {
        drawRadar(200, x * 12);
        draw(radarScreen);
        delay(15);
        clearScreen(radarScreen);
        draw(radarScreen);
    }
    //scan 1
    for (int x = 0; x <= 64; x++)
    {

        digitalWrite(pingPin, LOW);
        delayMicroseconds(2);
        digitalWrite(pingPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(pingPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        cm = microsecondsToCentimeters(duration);
        float angle = degree * x;
        if (cm <= 200)
        {
            Serial.print(cm);
            Serial.print("cm  ");
            Serial.print(angle);
            Serial.print("degrees");
            Serial.println();
            drawDistance(cm, angle);
        }
        else if (cm > 200)
        {
            Serial.println("OUT OF RANGE");
        }
        draw(detection);
        myStepper.step(stepsPerRevolution / 64 / 2);
    }
    clearScreen(detection);
    draw(detection);

    //scan 2 animation
    for (int x = 15; x >= 0; x--)
    {
        drawRadar(200, x * 12);
        draw(radarScreen);
        delay(15);
        clearScreen(radarScreen);
        draw(radarScreen);
    }
    //scan 2
    for (int x = 64; x >= 0; x--)
    {
        digitalWrite(pingPin, LOW);
        delayMicroseconds(2);
        digitalWrite(pingPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(pingPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        cm = microsecondsToCentimeters(duration);
        int angle = degree * x;
        if (cm <= 200)
        {
            Serial.print(cm);
            Serial.print("cm  ");
            Serial.print(angle);
            Serial.print("degrees");
            Serial.println();
            drawDistance(cm, angle);
        }
        else if (cm > 200)
        {
            Serial.println("OUT OF RANGE");
        }

        draw(detection);
        myStepper.step(-stepsPerRevolution / 64 / 2);
    }
    clearScreen(detection);
    draw(detection);
}

void drawRadar(int dist, float angle)
{
    float adjacent = dist * cos(angle * 1000 / 57296);
    int ledDist = abs(adjacent / 25);
    if (ledDist == 0)
    {
        for (int vert = 0; vert < dist / 25; vert++)
        {
            addtoScreen(0, vert, true, angle, radarScreen);
        }
    }
    for (int x = 0; x < ledDist; x++)
    {
        for (int check = 0; check < 10; check++)
        {
            float mapVal = ((x * 25) + (check * 2.5)) * (tan(angle * 1000 / 57296));
            int ledVal = map(mapVal, 0, 200, 0, 8);
            addtoScreen(x, abs(ledVal), true, angle, radarScreen);
        }
    }
    if (angle == 180)
    {
        radarScreen[7][0] = 1;
    }
}

void drawDistance(int dist, float angle)
{
    float adjacent = dist * cos(angle * 1000 / 57296);
    int ledX = abs(adjacent / 25);
    if (ledX == 0)
    {
        addtoScreen(0, (dist / 25) - 1, true, angle, detection);
    }
    float mapVal = (adjacent) * (tan(angle * 1000 / 57296));
    int ledVal = map(mapVal, 0, 200, 0, 8);
    addtoScreen(ledX - 1, abs(ledVal), true, angle, detection);
    if (angle == 180)
    {
        addtoScreen(ledX, 0, true, angle, detection);
    }
}

void addtoScreen(int x, int y, boolean state, float angle, int screen[8][15])
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

void clearScreen(int screen[8][15])
{
    for (int q = 0; q < 15; q++)
    {
        for (int l = 0; l < 8; l++)
        {
            screen[l][q] = 0;
        }
    }
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 15; x++)
        {

            if (x < 8)
            {
                lc.setLed(1, x, abs(y - 7), false);
            }
            else
            {
                lc.setLed(0, x - 8, abs(y - 7), false);
            }
        }
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

long microsecondsToCentimeters(long microseconds)
{
    return microseconds / 29 / 2;
}
