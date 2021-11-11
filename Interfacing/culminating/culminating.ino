#include <LedControl.h>

LedControl lc = LedControl(7, 9, 8, 1);

const int joy1x = A0;
const int joy1y = A1;
const int joy2x = A2;
const int joy2y = A3;

int x1Val, y1Val, x2Val, y2Val, x1Map, y1Map, x2Map, y2Map;

const int BUZZER_PIN = 4;
const int ECHO_PIN = 5;
const int TRIGGER_PIN = 6;
float duration, distance;

const int ON_LED = 2;
int onBtn;
bool onState;

void setup()
{
    Serial.begin(115200);
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);

    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);
}

void loop()
{
    // /************ Start US Measurement Section ***********/
    // digitalWrite(TRIGGER_PIN, LOW);
    // delayMicroseconds(2);
    // digitalWrite(TRIGGER_PIN, HIGH);
    // delayMicroseconds(10);
    // digitalWrite(TRIGGER_PIN, LOW);
    // duration = pulseIn(ECHO_PIN, HIGH);
    // distance = duration * 0.034 / 2;
    // /************** End US Measurement Section ***********/

    // onBtn = digitalRead(3);

    // x1Val = map(analogRead(joy1x), 0, 1023, 0, 510);
    // y1Val = map(analogRead(joy1y), 0, 1023, 0, 510);
    // x2Val = analogRead(joy2x);
    // y2Val = analogRead(joy2y);

    // if (onBtn == LOW)
    // {
    //     delay(200);
    //     onState = (onState) ? false : true;
    //     digitalWrite(ON_LED, (onState) ? HIGH : LOW);
    // }

    // if (onState)
    // {
    //     Serial.print("X1 : ");
    //     Serial.print(x1Val);
    //     Serial.print(" \t Y1 : ");
    //     Serial.print(y1Val);
    //     Serial.print(" \t X2 : ");
    //     Serial.print(x2Val);
    //     Serial.print(" \t Y2 : ");
    //     Serial.print(y2Val);
    //     Serial.print(" \t Dist : ");
    //     Serial.println(distance);
    // }
    // else
    // {
    //     noTone(BUZZER_PIN);
    // }

    x1Map = map(x1Val, 0, 510, 0, 8);
    y1Map = map(y1Val, 0, 510, 7, 0);
    lc.setLed(0, x1Map, y1Map, true);
    lc.setLed(0, x1Map, y1Map + 1, true);
    lc.clearDisplay(0);
}
