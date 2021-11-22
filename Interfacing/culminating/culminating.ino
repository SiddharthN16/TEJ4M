#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

LedControl matrix = LedControl(12, 11, 10, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte paddlePositions[6] =
    {B11100000,
     B01110000,
     B00111000,
     B00011100,
     B00001110,
     B00000111};
byte paddleVal[2] = {0x00, 0x00};

const int joy1y = A0;
const int joy2y = A1;

int y1Val, y2Val;
int pad1Pos, pad2Pos;

const int BUZZER_PIN = 5;

const int ON_LED = 2;
int onBtn;
bool onState;

bool canSpawn = true;

float ballX = 4, ballY = 4;
float speedX = 1, speedY = -1;

int prevPos;

int disp = 0;

int rightScore = 0;
int leftScore = 0;
int prevRight = rightScore;
int prevLeft = leftScore;

int stringStart, stringStop = 0;
int scrollCursor = 16;
String message = "Push the Button to Begin!";

void setup()
{
    Serial.begin(9600);

    pinMode(ON_LED, OUTPUT);

    //waking up, setting brightness and clearing both matrixes
    for (int i = 0; i < 2; i++)
    {
        matrix.shutdown(i, false);
        matrix.setIntensity(i, 12);
        matrix.clearDisplay(i);
    }

    //initalizing the lcd display
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void loop()
{
    onBtn = digitalRead(3);

    lcd.setCursor(0, 0);
    lcd.print("Welcome to PONG!");

    lcd.setCursor(scrollCursor, 1);
    lcd.print(message.substring(stringStart, stringStop));

    delay(200);
    lcd.clear();

    if (stringStart == 0 && scrollCursor > 0)
    {
        scrollCursor--;
        stringStop++;
    }
    else if (stringStart == stringStop)
    {
        stringStart = stringStop = 0;
        scrollCursor = 16;
    }
    else if (stringStop == message.length() && scrollCursor == 0)
    {
        stringStart++;
    }
    else
    {
        stringStart++;
        stringStop++;
    }

    if (onBtn == LOW)
    {
        delay(200);
        onState = (onState) ? false : true;           //ternary statement to which sets toggles onState between true & false
        digitalWrite(ON_LED, (onState) ? HIGH : LOW); // ternary statement which sets the led to high or low depending on onState being true
    }
    while (onState)
    {
        // reading inputs
        onBtn = digitalRead(3);
        y1Val = analogRead(joy1y);
        y2Val = analogRead(joy2y);

        // drawing the ball & paddles
        drawBall(disp, ballX, ballY);
        pad1Pos = createPaddle(y1Val, 1);
        pad2Pos = createPaddle(y2Val, 0);
        drawPaddles(paddleVal);

        lcd.setCursor(5, 0);
        lcd.print("SCORES");
        if (rightScore != prevRight)
        {
            lcd.setCursor(14, 1);
            lcd.print(rightScore);

            prevRight = rightScore;
        }

        if (leftScore != prevLeft)
        {
            lcd.setCursor(0, 1);
            lcd.print(leftScore);

            prevLeft = leftScore;
        }

        if (canSpawn)
        {
            spawn(disp, ballX, ballY, speedX, speedY);
            canSpawn = false;
        }

        // moving ball depending on the speed
        ballX += speedX;
        ballY += speedY;

        //when ball bounces of Bottom or Top of Screen
        if (ballY <= 0 || ballY >= 7)
        {
            speedY *= -1;
        }

        // when ball bounces of Left & Right [TEMP]
        if ((ballX < 0 && disp == 0) || (ballX > 7 && disp == 1))
        {
            if (ballX < 0 && disp == 0)
            {
                rightScore++;
            }
            else if (ballX > 7 && disp == 1)
            {
                leftScore++;
            }
            // resetting ball data to respawn the ball
            disp = 0;
            ballX = 7;
            ballY = 4;
            speedX = 1;
            speedY = 1;
            spawn(disp, ballX, ballY, speedX, speedY);
        }

        // determine if ball is above or below the paddle on Display 0
        if (disp == 0)
        {
            if (ballX == 1)
            {
                prevPos = ballY;
            }
        }
        // Determine if ball is above or below the paddle on Display 1
        else if (disp == 1)
        {
            if (ballX == 6)
            {
                prevPos = ballY;
            }
        }

        //ball bouncing off Left Paddle
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

        // ball bouncing of Right Paddle
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

        // reseting all variables to turn off the game
        if (onBtn == LOW)
        {
            delay(250);
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
            canSpawn = true;
            leftScore = 0;
            rightScore = 0;
            break;
        }
    }
}

// function to create the paddle positions
int createPaddle(int y, int paddle)
{
    int yPos = map(y, 0, 1000, 5, 0);
    paddleVal[paddle] = paddlePositions[yPos];

    return yPos;
}

// functions to draw both paddles
void drawPaddles(byte paddleVal[2])
{
    for (int i = 0; i < 2; i++)
    {
        matrix.setRow(i, i * 7, paddleVal[i]);
    }
}

// function to draw the ball
void drawBall(int disp, int ballX, int ballY)
{
    matrix.setLed(disp, ballX, ballY, true);
    delay(40);
    matrix.setLed(disp, ballX, ballY, false);
}

void spawn(int disp, int ballX, int ballY, int speedX, int speedY)
{

    // blink the ball when respawning the ball
    for (int i = 0; i < 8; i++)
    {
        matrix.setLed(disp, ballX, ballY, true);
        delay(100);
        matrix.setLed(disp, ballX, ballY, false);
        delay(100);
    }
}