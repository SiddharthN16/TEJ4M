#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 1);

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

int spawnX = random(3, 4);
int spawnY = random(0, 7);
float ballX = 2, ballY = 4;
int speedX = 1, speedY;

bool spawn = true;

void setup()
{
    Serial.begin(9600);
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);

    lc.shutdown(0, false);
    lc.setIntensity(0, 12);
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

    onBtn = digitalRead(3);

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

    if (onState)
    {
        // x1Val = analogRead(joy1x);
        y1Val = analogRead(joy1y);
        // x2Val = analogRead(joy2x);
        y2Val = analogRead(joy2y);

        ballMovement(ballX, ballY, speedX, speedY);

        // onState = collision(ballX, ballY, y1Val, y2Val);

        paddleRight(y1Val);
        paddleLeft(y2Val);
        lc.clearDisplay(0);
    }
    else
    {
        noTone(BUZZER_PIN);
    }
}

void paddleRight(int y)
{
    int yPos = map(y, 0, 1023, 6, -1);
    lc.setLed(0, 7, yPos, true);
    lc.setLed(0, 7, yPos + 1, true);
}

void paddleLeft(int y)
{
    int yPos = map(y, 0, 1023, 6, -1);
    lc.setLed(0, 0, yPos, true);
    lc.setLed(0, 0, yPos + 1, true);
    delay(55);
}

bool collision(int ballX, int ballY, int paddle1Y, int paddle2Y)
{
    if ((ballX == 0 || ballX == 7) && (ballY == paddle1Y || ballY == paddle2Y))
    {
        return false;
    }
    else if (ballX == 0 || ballX == 7)
    {
        ballX = 3;
    }
    else
    {
        return true;
    }
}

void ballMovement(int ballX, int ballY, int speedX, int speedY)
{
    lc.setLed(0, ballX += 1, ballY, true);
    ballX += 0.000001;
    delay(100);
    Serial.println(ballX);
}