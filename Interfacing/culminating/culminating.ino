/*
-----------------------------------------------------------------------------
Name:        Arduino Culminating - PONG
Purpose:     Custom Arduino Project

Author:      Siddharth Nema
Created:     11-Nov-2021
Updated:     22-Nov-2021
-----------------------------------------------------------------------------
*/

// libraries required
#include <LedControl.h>
#include <LiquidCrystal_I2C.h>

// initializing matrix and lcd objects
LedControl matrix = LedControl(12, 11, 10, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// possible positions for the paddles
byte paddlePositions[6] =
    {B11100000,
     B01110000,
     B00111000,
     B00011100,
     B00001110,
     B00000111};

// holds the position of the paddles
byte paddleVal[2] = {0x00, 0x00};

// analog pins where the joysticks are connected
const int joy1y = A0;
const int joy2y = A1;

int y1Val, y2Val;     // raw input values from the joysticks
int pad1Pos, pad2Pos; // joystick positions mapped to matrix

const int BUZZER_PIN = 5; // pin where the buzzer is connected

const int ON_LED = 2; // pin where the led is connected
int onBtn;            // reading the value of the button
bool onState;         // boolean to toggle the game

bool canSpawn = true; // boolean to know if the ball should respawn

// controlling the position & speed of the ball
float ballX = 4, ballY = 4;
float speedX = 1, speedY = -1;

int prevPos; // to know the previous position of the ball

int disp = 0; //  specifies the matrix display

// controlling the score
int rightScore = 0;
int leftScore = 0;
int prevRight = rightScore;
int prevLeft = leftScore;

// indexing the beginning & end of l`cd string
int stringStart, stringStop = 0;
int scrollCursor = 16;
String message = "Push the Button to Begin!"; // message to display on lcd

void setup()
{
    Serial.begin(9600);

    // setting pinmodes for led and buzzer
    pinMode(ON_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    //waking up, setting brightness and clearing both matrixes
    for (int i = 0; i < 2; i++)
    {
        matrix.shutdown(i, false);
        matrix.setIntensity(i, 12);
        matrix.clearDisplay(i);
    }

    //initializing the lcd display
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void loop()
{
    onBtn = digitalRead(3); // read the value of the button

    // printing the top line of lcd screen
    lcd.setCursor(0, 0);
    lcd.print("Welcome to PONG!");

    // printing the second scrolling text line of lcd
    lcd.setCursor(scrollCursor, 1);
    lcd.print(message.substring(stringStart, stringStop));

    delay(200);
    lcd.clear(); // clearing the display

    // moves the cursor until its 0
    if (stringStart == 0 && scrollCursor > 0)
    {
        scrollCursor--;
        stringStop++;
    }
    // resetting the variables to start scrolling again
    else if (stringStart == stringStop)
    {
        stringStart = stringStop = 0; // change later
        scrollCursor = 16;
    }
    // remove text when off screen
    else if (stringStop == message.length() && scrollCursor == 0)
    {
        stringStart++;
    }
    // to scroll in the background back to beginning
    else
    {
        stringStart++;
        stringStop++;
    }
    // when button is pressed
    if (onBtn == LOW)
    {
        delay(200);
        onState = (onState) ? false : true;           //ternary statement to which sets toggles onState between true & false
        digitalWrite(ON_LED, (onState) ? HIGH : LOW); // ternary statement which sets the led to high or low depending on onState being true
    }

    // playing the game when the button is first pressed
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

        // displaying the scores for each player
        lcd.setCursor(5, 0);
        lcd.print("SCORES");

        // only displaying scores when they are updated to save memory
        if (rightScore != prevRight)
        {
            lcd.setCursor(14, 1);
            lcd.print(rightScore);
            prevRight = rightScore;
        }
        // only displaying scores when they are updated to save memory
        if (leftScore != prevLeft)
        {
            lcd.setCursor(0, 1);
            lcd.print(leftScore);
            prevLeft = leftScore;
        }
        // when the game first startts spawn the ball
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
        // when ball bounces of Left & Right
        if ((ballX < 0 && disp == 0) || (ballX > 7 && disp == 1))
        {
            // if ball goes out on left side, add score to right player
            if (ballX < 0 && disp == 0)
            {
                rightScore++;
            }
            // if ball goes out on right side, add score to left player
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

            playTone(BUZZER_PIN, 200, 500);
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
                // bouncing off closest paddle point
                if (ballY == pad2Pos + 1)
                {
                    speedY *= random(-125, -95) / 95;
                }
                // bouncing off farthest paddle point
                else if (ballY == pad2Pos - 1)
                {
                    speedY *= random(175, 215) / 95;
                }
            }
            // when ball was below paddle
            else if (prevPos < pad2Pos)
            {
                //bouncing off closest paddle point
                if (ballY == pad2Pos - 1)
                {
                    speedY *= random(-125, -95) / 95;
                }
                //bouncing farthest paddle point
                else if (ballY == pad2Pos + 1)
                {
                    speedY *= random(175, 215) / 95;
                }
            }

            playTone(BUZZER_PIN, 750, 100);   // playing the tone when ball hits paddle
            speedX *= random(-125, -95) / 95; // reversing the x direction
        }

        // ball bouncing of Right Paddle
        else if ((ballX == 7 && (ballY == pad1Pos - 1 || ballY == pad1Pos || ballY == pad1Pos + 1)) && disp == 1)
        {
            // When ball was above paddle
            if (prevPos > pad1Pos)
            {
                //bouncing off closest paddle point
                if (ballY == pad1Pos + 1)
                {
                    speedY *= -1;
                }
                //bouncing off farthest paddle point
                else if (ballY == pad1Pos - 1)
                {
                    speedY *= 2;
                }
            }
            //When ball was below paddle
            else if (prevPos < pad1Pos)
            {
                //bouncing off closest paddle point
                if (ballY == pad1Pos - 1)
                {
                    speedY *= -1;
                }
                //bouncing off farthest paddle point
                else if (ballY == pad1Pos + 1)
                {
                    speedY *= 2;
                }
            }
            playTone(BUZZER_PIN, 750, 100);   // playing the tone when ball hits paddle
            speedX *= random(-125, -95) / 95; // reversing the x direction
        }
        //Switching between 2 Matrixes
        if ((ballX > 7 && disp == 0) || (ballX < 0 && disp == 1))
        {
            disp = (disp == 0) ? 1 : 0;
            ballX = (disp == 1) ? 0 : 7;
        }
        // resetting all variables to turn off the game
        if (onBtn == LOW)
        {
            delay(250);
            onState = false;
            digitalWrite(ON_LED, LOW); // turn off led

            noTone(BUZZER_PIN); // stop buzzer

            //clear matrices
            matrix.clearDisplay(0);
            matrix.clearDisplay(1);

            // resetting all variables
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
    // draw
    for (int i = 0; i < 2; i++)
    {
        matrix.setRow(i, i * 7, paddleVal[i]);
    }
}

// function to draw the ball
void drawBall(int disp, int ballX, int ballY)
{
    // draw ball at specified position
    matrix.setLed(disp, ballX, ballY, true);
    delay(40);
    matrix.setLed(disp, ballX, ballY, false);
}

// function to respawn the ball
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

// function to play a tone of any frequency for set amount of time
void playTone(int pin, int frequency, int time)
{
    tone(pin, frequency);
    delay(time);
    noTone(pin);
}