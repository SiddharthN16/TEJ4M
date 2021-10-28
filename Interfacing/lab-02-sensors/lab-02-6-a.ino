/*
-----------------------------------------------------------------
Name: US Sensor - Written by Mr.K 2021.10.16
Purpose: This code is written to sense distance and sound a buzzer
"tone" commands

Author: Siddharth Nema
Created: 2021.10.28
Updated: 2021.10.29
-----------------------------------------------------------------
*/

// declare variables for pins
int GREEN_LED_PIN = 4;
int RED_LED_PIN = 5;
int BUZZER_PIN = 6;
int TRIGGER_PIN = 3;
int ECHO_PIN = 2;

// declare variables used for calculations
float duration;
float distance;
float frequency;

void setup()
{
    // set pins 3 to 6 outputs
    for (int i = 3; i <= 6; i++)
    {
        pinMode(i, OUTPUT);
    }

    pinMode(2, INPUT); // set pin 2 as input
}

void loop()
{
    /************ Start US Measurement Section ***********/
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;
    /************** End US Measurement Section ***********/

    frequency = 1053 - distance * 9.57; // determine the frequency proportional to distance

    // frequency changes depending on distance
    if (distance > 0 && distance <= 110)
    {
        tone(BUZZER_PIN, frequency);
    }

    // when distance is not between 0 cm and 110 cm
    else
    {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        noTone(BUZZER_PIN);
    }

    delay(10); // small delay to save system resources
}
