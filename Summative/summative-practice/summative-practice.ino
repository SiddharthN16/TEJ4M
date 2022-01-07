/*
-----------------------------------------------------------------------------
Name:        Summative Practice
Purpose:     To complete the given project as practice for the final summative

Author:      Siddharth Nema
Created:     05-Jan-2022
Updated:     07-Jan-2022
-----------------------------------------------------------------------------
*/

// library for the lcd display
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, 0);

// all the pins for the led's
const int READY_LED_PIN = 1;
const int HEAVY_LED_PIN = 2;
const int WASH_LED_PIN = 3;
const int RINSE_LED_PIN = 4;
const int DONE_LED_PIN = 5;

// all the pins for the pushbuttons
const int heavyWashPB_PIN = 6;
const int washPB_PIN = 7;
const int rinsePB_PIN = 8;
const int stopPB_PIN = 9;

const int doorSwitch_PIN = 10; //pin for the ready switch

const int BUZZER_PIN = 11; // pin for the buzzer

const int VIBRATION_PIN = 12;

const int FORCE_PIN = A5;

int mode = 0; // used to switch the cases

bool readyKey = false; // used to check if washing machine is in ready mode

// delays for the washing machine cycles
int washTime = 5000;
int rinseTime = 3000;

float endNotes[] = {554.37, 783.99, 698.46, 622.25, 554.37, 466.16, 523.25, 554.37, 622.25, 415.30, 466.16, 523.25, 466.16, 554.37, 554.37, 783.99, 698.46, 622.25, 554.37, 783.99, 783.99, 880, 783.99, 698.46, 622.25, 698.46, 783.99, 0};

int endDelays[] = {225, 37.5, 37.5, 37.5, 225, 225, 37.5, 37.5, 37.5, 37.5, 37.5, 37.5, 225, 225, 225, 37.5, 37.5, 37.5, 225, 225, 37.5, 37.5, 37.5, 37.5, 37.5, 37.5, 300, 150};

float readyNotes[] = {659.25, 659.25, 659.25, 523.25, 783.99};
float readyDelays[] = {80, 80, 40, 40, 80};

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();

  // setup the led's as outputs
  pinMode(READY_LED_PIN, OUTPUT);
  pinMode(WASH_LED_PIN, OUTPUT);
  pinMode(RINSE_LED_PIN, OUTPUT);
  pinMode(DONE_LED_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);

  // setup the pushbuttons as inputs
  pinMode(doorSwitch_PIN, INPUT);
  pinMode(stopPB_PIN, INPUT);
  pinMode(rinsePB_PIN, INPUT);
  pinMode(washPB_PIN, INPUT);
  pinMode(FORCE_PIN, INPUT);
}

void loop()
{
  //read button states
  bool doorState = digitalRead(doorSwitch_PIN);
  bool heavyPBState = digitalRead(heavyWashPB_PIN);
  bool washPBState = digitalRead(washPB_PIN);
  bool rinsePBState = digitalRead(rinsePB_PIN);
  bool stopPBState = digitalRead(stopPB_PIN);

  int forceValue = analogRead(FORCE_PIN);

  // case structure which switches depending on the washing machine mode
  switch (mode)
  {
  case 0: // base case
    // when slide-switch is set to ready, display text & switch mode
    if (doorState == LOW && readyKey == false)
    {
      lcd.clear();
      lcd.print("READY!");

      forceValue = analogRead(FORCE_PIN);

      if (forceValue >= 300)
      {
        mode = 1;
      }
      else
      {
        playTone(400, 500);
      }
    }
    // otherwise keep all led's off
    else if (doorState == HIGH)
    {
      lcd.clear();
      digitalWrite(READY_LED_PIN, LOW);
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(HEAVY_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(DONE_LED_PIN, LOW);
      readyKey = false;
    }
    // when wash button is pressed, mode switched & tone played
    if (washPBState == LOW && readyKey == true)
    {
      playTone(350, 75);
      mode = 2;
    }
    // when rinse button is pressed, mode switched & tone played
    if (rinsePBState == LOW && readyKey == true)
    {
      playTone(350, 75);
      mode = 3;
    }
    // when heavy duty button is pressed, mode switched & tone played
    if (heavyPBState == LOW && readyKey == true)
    {
      playTone(350, 75);
      mode = 4;
    }

    break;

  case 1: // turning on ready light
    // only turn on ready led
    digitalWrite(READY_LED_PIN, HIGH);
    digitalWrite(WASH_LED_PIN, LOW);
    digitalWrite(RINSE_LED_PIN, LOW);
    digitalWrite(DONE_LED_PIN, LOW);

    // Loop through each note and delay for the ending music
    for (int i = 0; i <= 4; i++)
    {
      playTone(readyNotes[i], readyDelays[i]);
      delay(125);
    }

    readyKey = true;
    delay(150);
    mode = 0;
    break;

  case 2: // wash sequence
    lcd.clear();
    lcd.print("WASHING...");

    // start the vibration motor
    digitalWrite(VIBRATION_PIN, HIGH);

    // only turn on the wash led
    digitalWrite(READY_LED_PIN, LOW);
    digitalWrite(WASH_LED_PIN, HIGH);

    // initiates countdown sequence and changes mode
    mode = delayCountdown(washTime, doorState, stopPBState, mode);

    // breaks out of case if mode is for standby case
    if (mode == 6)
    {
      break;
    }

    lcd.clear();
    lcd.print("RINSING...");

    //only turn on rinse led
    digitalWrite(WASH_LED_PIN, LOW);
    digitalWrite(RINSE_LED_PIN, HIGH);

    // initiates countdown sequence and changes mode
    mode = delayCountdown(rinseTime, doorState, stopPBState, mode);

    // only turns on done led
    digitalWrite(RINSE_LED_PIN, LOW);
    digitalWrite(DONE_LED_PIN, HIGH);

    digitalWrite(VIBRATION_PIN, LOW);

    lcd.clear();
    break;

  case 3: // rinse sequence
    lcd.clear();
    lcd.print("RINSING...");

    // start the vibration motor
    digitalWrite(VIBRATION_PIN, HIGH);

    //only turn on rinse led
    digitalWrite(READY_LED_PIN, LOW);
    digitalWrite(RINSE_LED_PIN, HIGH);

    // initiates countdown sequence and changes mode
    mode = delayCountdown(rinseTime, doorState, stopPBState, mode);

    // only turns on done led
    digitalWrite(RINSE_LED_PIN, LOW);
    digitalWrite(DONE_LED_PIN, HIGH);

    digitalWrite(VIBRATION_PIN, LOW);
    lcd.clear();
    break;

  case 4: // heavy wash sequence
    lcd.clear();
    lcd.print("[H.D]:WASHING...");

    // start the vibration motor
    digitalWrite(VIBRATION_PIN, HIGH);

    //only turns on heavy duty & wash led
    digitalWrite(READY_LED_PIN, LOW);
    digitalWrite(HEAVY_LED_PIN, HIGH);
    digitalWrite(WASH_LED_PIN, HIGH);

    // initiates countdown sequence and changes mode
    mode = delayCountdown(washTime * 2, doorState, stopPBState, mode);

    // breaks out of case if mode is for standby case
    if (mode == 6)
    {
      break;
    }

    lcd.clear();
    lcd.print("[H.D]:RINSING...");

    // only keeps heavy duty & rinse led
    digitalWrite(WASH_LED_PIN, LOW);
    digitalWrite(RINSE_LED_PIN, HIGH);

    // initiates countdown sequence and changes mode
    mode = delayCountdown(rinseTime, doorState, stopPBState, mode);

    // only keeps heavy duty & done led
    digitalWrite(RINSE_LED_PIN, LOW);
    digitalWrite(DONE_LED_PIN, HIGH);

    digitalWrite(VIBRATION_PIN, LOW);
    lcd.clear();
    break;

  case 5: // turn on done light
    lcd.print("FINISHED!");

    // Loop through each note and delay for the ending music
    for (int i = 0; i <= 26; i++)
    {
      playTone(endNotes[i], endDelays[i]);
      delay(125);
    }

    // keeps done led on
    digitalWrite(READY_LED_PIN, LOW);
    digitalWrite(WASH_LED_PIN, LOW);
    digitalWrite(RINSE_LED_PIN, LOW);
    digitalWrite(DONE_LED_PIN, HIGH);

    digitalWrite(VIBRATION_PIN, LOW);

    // wait until the stop button is pressed to prevent music from replaying
    while (true)
    {
      // checking if stop button is pressed or door is closed to switch to standby mode
      stopPBState = digitalRead(stopPB_PIN);
      doorState = digitalRead(doorSwitch_PIN);

      // if stop button pressed, change mode to standby mode
      if (stopPBState == LOW || doorState == HIGH)
      {
        playTone(350, 75);
        mode = 6;
        break;
      }
    }

    noTone(BUZZER_PIN);
    delay(150);
    lcd.clear();
    break;

  case 6: // standby mode
    // turns off all leds
    digitalWrite(READY_LED_PIN, LOW);
    digitalWrite(WASH_LED_PIN, LOW);
    digitalWrite(HEAVY_LED_PIN, LOW);
    digitalWrite(RINSE_LED_PIN, LOW);
    digitalWrite(DONE_LED_PIN, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
    lcd.clear();

    // Check if door is opened to reset the program
    doorState = digitalRead(doorSwitch_PIN);
    if (doorState == HIGH)
    {
      mode = 0;
      readyKey = false;
      break;
    }

    noTone(BUZZER_PIN);
  }
}

// function to create countdown sequence & check if stop or door open switches are pressed
int delayCountdown(int delayTime, bool doorState, bool stopPBState, int mode)
{
  // loops through the length of the specified delay time
  for (int i = 0; i < delayTime; i += 50)
  {
    // checks the state of the door & stop switches
    doorState = digitalRead(doorSwitch_PIN);
    stopPBState = digitalRead(stopPB_PIN);

    // if either is pressed, immediately break out of loop and switch to standby case
    if (doorState == HIGH || stopPBState == LOW)
    {
      playTone(350, 75);
      mode = 6;
      break;
    }

    // on second line of led display the time left in the cycle
    lcd.setCursor(0, 1);
    lcd.print((delayTime + 800 - i) / 1000);
    lcd.setCursor(3, 1);
    lcd.print("Sec. Left");
    delay(50);

    // clearing a certain character
    lcd.setCursor(1, 1);
    lcd.print(" ");
    mode = 5;
  }
  return mode;
}

// function to play the buzzer for a specific tone & duration
void playTone(int frequency, int duration)
{
  tone(BUZZER_PIN, frequency);
  delay(duration);
  noTone(BUZZER_PIN);
}