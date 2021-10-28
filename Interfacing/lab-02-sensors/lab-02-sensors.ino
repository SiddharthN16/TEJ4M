/* US Sensor - Written by Mr.K 2021.10.16
   This code is written to sense distance and sound a buzzer
   "tone" commands */

// declare variables for pins
int GREEN_LED_PIN = 4;
int RED_LED_PIN = 5;
int BUZZER_PIN = 6;
int TRIGGER_PIN = 3;
int ECHO_PIN = 2;

float duration;
float distance;

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

    // if distance is less than 50cm
    if (distance <= 110 && distance >= 90)
    {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        tone(BUZZER_PIN, 440);
    }

    if (distance < 90 && distance > 70)
    {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        tone(BUZZER_PIN, 340);
    }

    delay(10); // small delay to save system resources
}
