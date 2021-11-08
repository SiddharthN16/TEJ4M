/* LCD - Written by Mr.K 2021.10.16
   This code is written to display values to an LCD display
   libraries, LCD library commands */

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // initialize the LCD with pin numbers

void setup()
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
}

void loop()
{
  /* print a message to the LCD starting at column 0, line 0
  and wait 1 second*/
  lcd.setCursor(0, 0);
  lcd.print("Self destruct in:");
  delay(1000);

  /* print numbers 5, 4, 3, 2, 1 every second
  starting at column 0 line 1*/
  for (int n = 5; n > 0; n--)
  {
    lcd.setCursor(0, 1);
    lcd.print(n);
    delay(1000);
  }

  //clear LCD and flash BOOM on and off
  for (int n = 10; n > 0; n--)
  {
    lcd.clear();
    delay(200);
    lcd.print("BOOM!");
    delay(200);
  }

  //print just kidding to LCD
  lcd.clear();
  lcd.print("just kidding");

  while (true) // loop forever
  {
  }
}
