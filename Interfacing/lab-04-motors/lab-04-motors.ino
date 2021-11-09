/* motorPins - Written by Siddharth Nema 2021.11.09
   This code is written to drive two motorPinss forward, then reverse in a loop
   constant, motorPins control */

const int motorPins[] = {5, 6, 9, 10}; //Declare constants for motorPins pins

void setup()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(motorPins[i], OUTPUT); //Sets all the motor pins to output
    }
}

void loop()
{
    forward(255, 3000);
    reverse(150, 2000);
    stop(1000);
    reverse(75, 3500);
    stop(2000);
    forward(100, 2500);
}

void forward(int speed, int time) //Spins the motors forward, at a controllable speed
{
    analogWrite(motorPins[0], speed);
    digitalWrite(motorPins[1], LOW);
    analogWrite(motorPins[2], speed);
    digitalWrite(motorPins[3], LOW);
    delay(time);
}

void reverse(int speed, int time) //Spins the motors in reverse, at a controllable speed
{
    digitalWrite(motorPins[0], LOW);
    analogWrite(motorPins[1], speed);
    digitalWrite(motorPins[2], LOW);
    analogWrite(motorPins[3], speed);
    delay(time);
}

void stop(int time) // Stops the motors
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(motorPins[i], LOW);
    }
    delay(time);
}