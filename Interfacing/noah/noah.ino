#include <LiquidCrystal.h>

int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, temp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int blue = 255;
int red = 255;
int bluePin = 11;
int redPin = 8;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup()
{
    Serial.begin(9600);

    pinMode(bluePin, OUTPUT); //blue
    pinMode(redPin, OUTPUT);  //red
}

void loop()
{

    Vo = analogRead(ThermistorPin);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    temp = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    temp = temp - 273.15;

    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");

    if (temp >= 25 && temp <= 27)
    {
        analogWrite(bluePin, 255);
        analogWrite(redPin, 255);
    }

    if (temp < 25)
    {
        analogWrite(bluePin, 255);
        fade(redPin);
    }

    else if (temp > 27)
    {
        analogWrite(redPin, 255);
        fade(bluePin);
    }
    delay(500);
    lcd.clear();
}

void fade(int pin)
{
    for (int i = 0; i < 255; i++)
    {
        analogWrite(pin, 255 - i);
        delay(10);
    }
}
