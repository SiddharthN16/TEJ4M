/* motorPins - Written by Siddharth Nema 2021.11.09
   This code is written to drive two motorPinss forward, then reverse in a loop
   constant, motorPins control */

const int motorPins[] = {5, 6, 9, 10}; //Declare constants for motorPins pins
const int motorR1 = 5;
const int motorR2 = 6;
const int motorL1 = 9;
const int motorL2 = 10;

void setup()
{
    pinMode(motorR1, OUTPUT);
    pinMode(motorR2, OUTPUT);
    pinMode(motorL1, OUTPUT);
    pinMode(motorL2, OUTPUT);
}

void loop()
{
    motorMove(255, 0, 255, 0, 2000);
}

void motorMove(int rVal1, int rVal2, int lVal1, int lVal2, int time)
{
    analogWrite(motorR1, rVal1);
    analogWrite(motorR2, rVal2);
    analogWrite(motorL1, lVal1);
    analogWrite(motorL2, lVal2);
    delay(time);
}