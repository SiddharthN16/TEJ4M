/*
  -----------------------------------------------------------------
  Name: Pushbuttons
  Purpose: This code is written to display the state of a pushbutton
   digitalRead
  Author: Siddharth Nema
  Created: 2021.11.08
  Updated: 2021.11.08
  -----------------------------------------------------------------
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // initialize the LCD with pin numbers

int button1Pin = 8; //set pin where button 1 is wired
int button2Pin = 9; //set pin where button 2 is wired

int button1State; //create variable for the state of button 1
int button2State; //create variable for the state of button 2

void setup()
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
}

void loop()
{
  button1State = digitalRead(button1Pin); //read state of pushbutton1
  button2State = digitalRead(button2Pin); //read state of pushbutton2

  //When both buttons are pressed
  if (button1State == LOW && button2State == LOW)
  {
    lcd.clear();               // clear the screen of any content
    lcd.setCursor(0, 0);       // where text should start
    lcd.print("Both Pressed"); // text to be displayed
  }

  //When button1 is pressed
  else if (button1State == LOW)
  {
    lcd.clear();              // clear the screen of any content
    lcd.setCursor(0, 0);      // where text should start
    lcd.print("PB1 Pressed"); // text to be displayed
  }

  //When button2 is pressed
  else if (button2State == LOW)
  {
    lcd.clear();              // clear the screen of any content
    lcd.setCursor(0, 1);      // where text should start
    lcd.print("PB2 Pressed"); // text to be displayed
  }

  //When no button is pressed
  else if (button1State == HIGH && button2State == HIGH)
  {
    lcd.clear();                     // clear the screen of any content
    lcd.setCursor(0, 0);             // where text should start
    lcd.print("No Buttons Pressed"); // text to be displayed
  }

  delay(100); //delay to allow screen to update
}