/*
  -----------------------------------------------------------------
  Name: US Sensor
  Purpose: This code is written to sense distance and sound a buzzer
  "tone" commands

  Author: Siddharth Nema
  Created: 2021.10.28
  Updated: 2021.10.29
  -----------------------------------------------------------------
*/

/*
NOTES:
A4: 830
G4: 740
F: 698
E: 658
D: 586
C: 522
B: 492
A3: 440
F3: 348
*/

// declare variables for pins
int GREEN_LED_PIN = 4;
int RED_LED_PIN = 5;
int BUZZER_PIN = 6;
int TRIGGER_PIN = 3;
int ECHO_PIN = 2;

float duration;
float distance;

int musicNotes[] = {
    586,
    658,
    440,
    658,
    740,
    440,
    392,
    698,
    658,
    586,
    658,
    440,

    492,
    522,
    586,
    492,

    698,
    698,

    658,

    440,
    492,
    522,
    440,

    440,
    492,
    522,
    522,
    492,

    492,
    522,
    586,
    492,

    586,

    658,
    522,

    492,

    440,

    348,

    658,

    586,

};
int musicDelays[] = {
    750,
    750,
    500,
    750,
    750,
    125,
    125,
    125,
    125,
    750,
    750,
    1500,
    125, 125, 125, 125, 375, 375, 750, 125, 125, 125, 125, 375, 375, 375, 125, 250, 125, 125, 125, 125, 500, 250, 250, 250, 500, 250, 500, 1000};

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

    // if distance is between 110 cm and 90 cm
    if (distance <= 110 && distance >= 90)
    {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        tone(BUZZER_PIN, 440);
    }

    // if distance is between 90 cm and 70 cm
    else if (distance < 90 && distance >= 70)
    {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        tone(BUZZER_PIN, 640);
    }

    // if distance is between 70 cm and 50 cm
    else if (distance < 70 && distance > 50)
    {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        tone(BUZZER_PIN, 840);
    }

    // if distance is less than 50 cm
    else if (distance <= 50 && distance >= 0)
    {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);

        // loops through the notes and delays of the song
        for (int i = 0; i < 40; i++)
        {
            tone(BUZZER_PIN, musicNotes[i]);
            delay(musicDelays[i]);

            noTone(BUZZER_PIN);
            delay(15);
        }
    }

    // any other distance value
    else
    {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        noTone(BUZZER_PIN);
    }

    delay(10); // small delay to save system resources
}
