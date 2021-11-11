#include <LedControl.h>

const int joy1x = 0;
const int joy1y = 1;
const int joy2x = 2;
const int joy2y = 3;

int x1Val, y1Val, x2Val, y2Val;

const int BUZZER_PIN = 4;
const int ECHO_PIN = 6;
const int TRIGGER_PIN = 6;
float duration, distance;

const int ON_LED = 7;
int onBtn;
bool onState;

void setup()
{
    Serial.begin(9600);
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
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

    onBtn = digitalRead(2);

    x1Val = analogRead(joy1x);
    y1Val = analogRead(joy1y);
    x2Val = analogRead(joy2x);
    y2Val = analogRead(joy2y);

    if (onBtn == LOW)
    {
        delay(200);
        onState = (onState) ? false : true;
        digitalWrite(ON_LED, (onState) ? HIGH : LOW);
    }

    if (onState && distance <= 40)
    {
        Serial.print("X1 : ");
        Serial.print(x1Val);
        Serial.print(" \t Y1 : ");
        Serial.print(y1Val);
        Serial.print(" \t X2 : ");
        Serial.print(x2Val);
        Serial.print(" \t Y2 : ");
        Serial.print(y2Val);
        Serial.print(" \t Dist : ");
        Serial.println(distance);

        tone(BUZZER_PIN, 20);
        delay(50);
    }
    else
    {
        noTone(BUZZER_PIN);
    }

    delay(100);
}