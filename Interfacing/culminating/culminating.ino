#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 2);

byte paddles[] =
    {
        B11000000,
        B01100000,
        B00110000,
        B00011000,
        B00001100,
        B00000110,
        B00000011};

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
float ballX = random(1, 6), ballY = 4;
int speedX = 1, speedY = random(-1, 1);

bool spawn = true;

int disp = random(0, 1);

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

    lc.shutdown(1, false);
    lc.setIntensity(1, 12);
    lc.clearDisplay(1);
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

    // x1Val = analogRead(joy1x);
    y1Val = analogRead(joy1y);
    // x2Val = analogRead(joy2x);
    y2Val = analogRead(joy2y);

    if (onState)
    {

        ballX += speedX;
        ballY += speedY;

        if (ballX < 0 && disp == 0)
        {
            speedX *= -1;
        }

        if ((ballX >= 7 && disp == 0) || (ballX <= 0 && disp == 1))
        {
            disp = (disp == 0) ? 1 : 0;
            ballX = (disp == 1) ? 0 : 7;
        }

        if (ballX > 7 && disp == 1)
        {
            speedX *= -1;
        }

        if (ballY <= 0 || ballY >= 7)
        {
            speedY *= -1;
        }

        Serial.println(ballX);
        Serial.println(disp);
        // Serial.println(disp);
        lc.setLed(disp, ballX, ballY, true);
        delay(random(40, 65));
        lc.setLed(disp, ballX, ballY, false);
        // onState = collision(ballX, ballY, y1Val, y2Val);

        paddleRight(y1Val);
        paddleLeft(y2Val);
        // delay(55);
    }
    else
    {
        noTone(BUZZER_PIN);
        lc.clearDisplay(0);
        lc.clearDisplay(1);
    }
}

void paddleRight(int y)
{
    int yPos = map(y, 0, 1023, 0, 7);
    lc.setRow(1, 7, paddles[yPos]);
    // delay(55);
}

void paddleLeft(int y)
{
    int yPos = map(y, 0, 1023, 0, 7);
    lc.setRow(0, 0, paddles[yPos]);
    // delay(55);
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
}