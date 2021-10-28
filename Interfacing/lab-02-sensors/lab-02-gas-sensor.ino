/*
-----------------------------------------------------------------
Name: Gas Sensor
Purpose: This code is written to sense the level of gas and light LEDs
“analogRead” command, if..else statements

Author: Siddharth Nema
Created: 2021.10.28
Updated: 2021.10.29
-----------------------------------------------------------------
*/

// declare variables for pins
int GREEN_LED_PIN = 2;
int RED_LED_PIN = 3;
int ORANGE_LED_PIN = 4;
int Sensor_PIN = 0;

void setup()
{
    // set LED pins as outputs
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(ORANGE_LED_PIN, OUTPUT);
}

void loop()
{
    int sensorValue = analogRead(Sensor_PIN); // read sensor on analog input pin

    // if sensor reading is greater than 40
    if (sensorValue >= 40)
    {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(ORANGE_LED_PIN, LOW);
    }

    // if sensor is between 25 and 40
    else if (sensorValue < 40 && sensorValue >= 25)
    {
        digitalWrite(ORANGE_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
    }

    // if sensor is less than 25
    else
    {
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(ORANGE_LED_PIN, LOW);
    }

    delay(10); // small delay to save system resources
}
