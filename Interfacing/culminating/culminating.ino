#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

LedControl matrix = LedControl(12, 11, 10, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte paddles[] =
    {
        B11100000,
        B01110000,
        B00111000,
        B00011100,
        B00001110,
        B00000111};

const int joy1x = A0;
const int joy1y = A1;
const int joy2x = A2;
const int joy2y = A3;

int x1Val, y1Val, x2Val, y2Val;
int pad1Pos, pad2Pos;

const int BUZZER_PIN = 4;
const int ECHO_PIN = 5;
const int TRIGGER_PIN = 6;
float duration, distance;

const int ON_LED = 2;
int onBtn;
bool onState;

float ballX = 4, ballY = 4;
int spawnX = random(3, 4);
int spawnY = random(0, 7);
float speedX = 1, speedY = -1;

int prevPos;

bool spawn = true;

int disp = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);

    matrix.shutdown(0, false);
    matrix.setIntensity(0, 12);
    matrix.clearDisplay(0);

    matrix.shutdown(1, false);
    matrix.setIntensity(1, 12);
    matrix.clearDisplay(1);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello World!");
}

void loop()
{

    onBtn = digitalRead(3);

    if (onBtn == LOW)
    {
        delay(200);
        onState = (onState) ? false : true;
        digitalWrite(ON_LED, (onState) ? HIGH : LOW);
    }
    while (onState)
    {
        onBtn = digitalRead(3);
        x1Val = analogRead(joy1x);
        y1Val = analogRead(joy1y);
        x2Val = analogRead(joy2x);
        y2Val = analogRead(joy2y);

        ballX += speedX;
        ballY += speedY;

        Serial.print("SpeedX: ");
        Serial.print(speedX);
        Serial.print("    \t SpeedY: ");
        Serial.println(speedY);

        //Bounces of Bottom or Top of Screen
        if (ballY <= 0 || ballY >= 7)
        {
            speedY *= -1;
        }

        //Bounces of Left & Right [TEMP]
        if ((ballX < 0 && disp == 0) || (ballX > 7 && disp == 1))
        {
            disp = 0;
            ballX = 4;
            ballY = 4;
            speedX = 1;
            speedY = 1;
        }

        //Determine if ball is above or below the paddle on Display 0
        if (disp == 0)
        {
            if (ballX == 1)
            {
                prevPos = ballY;
            }
        }
        //Determine if ball is above or below the paddle on Display 1
        else if (disp == 1)
        {
            if (ballX == 6)
            {
                prevPos = ballY;
            }
        }

        //Bouncing of Left Paddle
        if ((ballX == 0 && (ballY == pad2Pos - 1 || ballY == pad2Pos || ballY == pad2Pos + 1)) && disp == 0)
        {
            // When ball was above paddle
            if (prevPos > pad2Pos)
            {
                //Closest paddle point
                if (ballY == pad2Pos + 1)
                {
                    speedY *= random(-125, -95) / 95;
                }
                //Farthest paddle point
                else if (ballY == pad2Pos - 1)
                {
                    speedY *= random(175, 215) / 95;
                }
            }
            //When ball was below paddle
            else if (prevPos < pad2Pos)
            {
                //Closest paddle point
                if (ballY == pad2Pos - 1)
                {
                    speedY *= random(-125, -95) / 95;
                }
                //Farthest paddle point
                else if (ballY == pad2Pos + 1)
                {
                    speedY *= random(175, 215) / 95;
                }
            }
            speedX *= random(-125, -95) / 95;
        }

        //Bouncing of Right Paddle
        else if ((ballX == 7 && (ballY == pad1Pos - 1 || ballY == pad1Pos || ballY == pad1Pos + 1)) && disp == 1)
        {
            // When ball was above paddle
            if (prevPos > pad1Pos)
            {
                //Closest paddle point
                if (ballY == pad1Pos + 1)
                {
                    speedY *= -1;
                }
                //Farthest paddle point
                else if (ballY == pad1Pos - 1)
                {
                    speedY *= 2;
                }
            }
            //When ball was below paddle
            else if (prevPos < pad1Pos)
            {
                //Closest paddle point
                if (ballY == pad1Pos - 1)
                {
                    speedY *= -1;
                }
                //Farthest paddle point
                else if (ballY == pad1Pos + 1)
                {
                    speedY *= 2;
                }
            }
            speedX *= -1;
        }

        //Switching between 2 Matrixes
        if ((ballX > 7 && disp == 0) || (ballX < 0 && disp == 1))
        {
            disp = (disp == 0) ? 1 : 0;
            ballX = (disp == 1) ? 0 : 7;
        }

        matrix.setLed(disp, ballX, ballY, true);
        delay(random(40, 65));
        matrix.setLed(disp, ballX, ballY, false);

        pad1Pos = paddleRight(y1Val) + 1;
        pad2Pos = paddleLeft(y2Val) + 1;

        if (onBtn == LOW)
        {
            delay(200);
            onState = false;
            digitalWrite(ON_LED, LOW);

            noTone(BUZZER_PIN);
            matrix.clearDisplay(0);
            matrix.clearDisplay(1);

            disp = 0;
            ballX = 4;
            ballY = 4;
            speedX = 1;
            speedY = -1;
            break;
        }
    }
}

int paddleRight(int y)
{
    int yPos = map(y, 0, 1000, 5, 0);
    matrix.setRow(1, 7, paddles[yPos]);
    // delay(55);
    return yPos;
}

int paddleLeft(int y)
{
    int yPos = map(y, 0, 1000, 5, 0);
    matrix.setRow(0, 0, paddles[yPos]);
    // delay(55);
    return yPos;
}