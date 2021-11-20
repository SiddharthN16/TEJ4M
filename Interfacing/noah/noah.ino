#include <LiquidCrystal.h>

int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, temp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int blue = 255;
int red = 255;
int bluePin = 11;
int redPin = 9;

int counter = 0;

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

    for (int i = 0; i < 10; i++)
    {
        Vo = analogRead(ThermistorPin);
        R2 = R1 * (1023.0 / (float)Vo - 1.0);
        logR2 = log(R2);
        temp = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
        temp = temp - 273.15;

        if (counter == 0)
        {
        }

        if (blue <= 0)
        {
            blue = 0;
        }

        if (red <= 0)
        {
            red = 0;
        }

        if (blue >= 255)
        {
            blue = 255;
        }

        if (red >= 255)
        {
            red = 255;
        }

        analogWrite(bluePin, blue);
        analogWrite(redPin, red);

        if (temp >= 26 && temp <= 29)
        {
            red += 15;
            blue += 15;
        }

        if (temp < 26)
        {
            red -= 15;
            blue += 15;
        }

        if (temp > 29)
        {
            red += 15;
            blue -= 15;
        }

        if (counter >= 10)
        {
            counter = 0;
        }
        counter++;
        delay(150);
        Serial.println(red);
        Serial.println(blue);
    }

    lcd.clear();
}