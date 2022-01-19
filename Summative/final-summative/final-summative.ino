
// libarries required
#include <LiquidCrystal.h>
#include <Servo.h>

// initializing display + servo
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
Servo servo1;

// pins for the pushbuttons
const int firstfloorPB_PIN = 0;
const int secondfloorPB_PIN = 1;
const int thirdfloorPB_PIN = 2;

// motor pins
const int motorIn1_PIN = 3;
const int motorIn2_PIN = 5;

const int servo_PIN = 6; // servo pin

const int BUZZER_PIN = 9; // buzzer pins

// echo + trigger pin for US sensor
const int ECHO_PIN = 10;
const int TRIGGER_PIN = 11;

int mode = 1; // the current mode the case structure is in

int currFloor = 1; // the elevator floor

int travelDelay = 5000; // time to travel through floors

// variables for US sensor
float duration;
float distance;

bool doorCheck = false; // checking if something is in the way of the door closing

void setup()
{
  // start the lcd
  lcd.begin(16, 2);
  lcd.clear();

  // setup pushbuttons as inputs
  pinMode(firstfloorPB_PIN, INPUT);
  pinMode(secondfloorPB_PIN, INPUT);
  pinMode(thirdfloorPB_PIN, INPUT);

  // attatch the servo to pin
  servo1.attach(servo_PIN);

  // set motor as outputs
  pinMode(motorIn2_PIN, OUTPUT);
  pinMode(motorIn1_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT); // set buzzer as output

  // setup US sensor for sending and receiving
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
}

void loop()
{
  //read button states
  bool firstfloorPBState = digitalRead(firstfloorPB_PIN);
  bool secondfloorPBState = digitalRead(secondfloorPB_PIN);
  bool thirdfloorPBState = digitalRead(thirdfloorPB_PIN);

  // switch case structure to go through different modes
  switch (mode)
  {
  // "default" case to check the state of buttons and go to required mode
  case 0:
    // first floor button only works when elevator is not on first floor
    if (firstfloorPBState == LOW && (currFloor == 2 || currFloor == 3))
    {
      mode = 2;
    }
    // second floor button only works when elevator is not on second floor
    if (secondfloorPBState == LOW && (currFloor == 1 || currFloor == 3))
    {
      mode = 3;
    }
    // third floor button only works when elevator is not on third floor
    if (thirdfloorPBState == LOW && (currFloor == 1 || currFloor == 2))
    {
      mode = 4;
    }
    break;

  // mode when the program starts
  case 1:
    // starts at first floor with door open & motors off
    lcdText(0, 0, "----1ST FLOOR---");
    lcdText(0, 1, "    Door Open   ");

    servo1.write(95);
    digitalWrite(motorIn1_PIN, LOW);
    digitalWrite(motorIn2_PIN, LOW);

    mode = 0;
    break;

  // When first floor button is pressed
  case 2:
    // play door closing warning
    playTone(600, 250);
    playTone(800, 500);
    delay(500);

    // check is there is something in the doors way
    doorCheck = moveServo(0, mode);

    // if something is blocking the door, keep door open
    if (doorCheck == true)
    {
      mode = 0;
      break;
    }

    // otherwise door closes successfully
    lcdText(0, 1, "   Door Closed  ");

    // time to second floor is five seconds
    if (currFloor == 2)
    {
      travelDelay = 5000;
    }
    //time to third floor is ten seconds
    if (currFloor == 3)
    {
      travelDelay = 10000;
    }

    // will always be going down to reach first floor from other floors
    lcdText(0, 0, "---Going Down---");

    // spin motors in reverse
    spinMotor(-1, travelDelay);

    lcd.clear();

    // stop motors when arrived at first floor
    spinMotor(0, travelDelay);
    lcdText(0, 0, "----1ST FLOOR---");

    // open the door
    currFloor = moveServo(95, mode);
    lcdText(0, 1, "    Door Open   ");

    mode = 0;
    break;

  // When second floor button is pressed
  case 3:
    // play door closing warning
    playTone(600, 250);
    playTone(800, 500);
    delay(500);

    // check if something is blocking door closing, if so keep door open
    doorCheck = moveServo(0, mode);
    if (doorCheck == true)
    {
      mode = 0;
      break;
    }

    lcdText(0, 1, "   Door Closed  ");

    // if coming from the first floor, elevator is going up & motor is spinning forward
    if (currFloor == 1)
    {
      lcdText(0, 0, "----Going Up----");
      spinMotor(1, travelDelay);
    }
    // if coming from third floor, elevator is going down & motor is in reverse
    else if (currFloor == 3)
    {
      lcdText(0, 0, "---Going Down---");
      spinMotor(-1, travelDelay);
    }

    lcd.clear();

    // stop the motor when on second floor
    spinMotor(0, travelDelay);
    lcdText(0, 0, "----2ND FLOOR---");

    // open the door
    currFloor = moveServo(95, mode);
    lcdText(0, 1, "    Door Open   ");

    mode = 0;
    break;

  // // When third floor button is pressed
  case 4:
    // play door closing warning
    playTone(600, 250);
    playTone(800, 500);
    delay(500);

    // check is something is blocking door, if so keep door open
    doorCheck = moveServo(0, mode);
    if (doorCheck == true)
    {
      mode = 0;
      break;
    }

    lcdText(0, 1, "   Door Closed  ");

    // travel time from first floor is ten seconds
    if (currFloor == 1)
    {
      travelDelay = 10000;
    }
    // travel time from second floor is five seconds
    else if (currFloor == 2)
    {
      travelDelay = 5000;
    }

    // elevator always going up to third floor, so motor always spinning forward
    lcdText(0, 0, "----Going Up----");
    spinMotor(1, travelDelay);

    lcd.clear();

    // stop motor when on third floor
    spinMotor(0, travelDelay);
    lcdText(0, 0, "----3RD FLOOR---");

    // open the door
    currFloor = moveServo(95, mode);
    lcdText(0, 1, "    Door Open   ");

    mode = 0;
    break;
  }
}

// function to spin the motor with acceleration & deceleration
void spinMotor(int direction, int travelDelay)
{
  // when the motor is spinning forwards
  if (direction == 1)
  {
    // grdually increase motor speed when starting
    for (int i = 1500; i >= 0; i -= 100)
    {
      analogWrite(motorIn1_PIN, map(i, 1500, 0, 500, 0));
      analogWrite(motorIn2_PIN, 500);
      delay(100);
    }

    // after 1.5 seconds motor at full speed
    digitalWrite(motorIn1_PIN, LOW);
    digitalWrite(motorIn2_PIN, HIGH);
    delay(travelDelay - 3000);

    // during last 1.5 seconds, motor gradually slows down
    for (int i = 0; i <= 1500; i += 100)
    {
      analogWrite(motorIn1_PIN, map(i, 0, 1500, 0, 500));
      analogWrite(motorIn2_PIN, 500);
      delay(100);
    }
  }
  // when motor is spinning in reverse
  else if (direction == -1)
  {
    // grdually increase motor speed when starting in opposite direction
    for (int i = 1500; i >= 0; i -= 100)
    {
      analogWrite(motorIn1_PIN, 500);
      analogWrite(motorIn2_PIN, map(i, 1500, 0, 500, 0));
      delay(100);
    }

    // after 1.5 seconds motor at full speed in opposite direction
    digitalWrite(motorIn1_PIN, HIGH);
    digitalWrite(motorIn2_PIN, LOW);
    delay(travelDelay - 3000);

    // during last 1.5 seconds, motor gradually slows down in opposite direction
    for (int i = 0; i <= 1500; i += 100)
    {
      analogWrite(motorIn1_PIN, 500);
      analogWrite(motorIn2_PIN, map(i, 0, 1500, 0, 500));
      delay(100);
    }
  }
  // stopping the motors
  else if (direction == 0)
  {
    digitalWrite(motorIn1_PIN, LOW);
    digitalWrite(motorIn2_PIN, LOW);
  }
}

// function to print text at a given point on display
void lcdText(int column, int row, String text)
{
  lcd.setCursor(column, row);
  lcd.print(String(text));
}

// function to play buzzer at given frequency and duration
void playTone(int frequency, int duration)
{
  tone(BUZZER_PIN, frequency);
  delay(duration);
  noTone(BUZZER_PIN);
}

// function to open/close the door (servo)
int moveServo(int degree, int mode)
{
  // when the door is opening
  if (degree == 95)
  {
    // gradually open the door
    for (int i = 0; i <= degree; i += 5)
    {
      servo1.write(i);
      delay(100);
    }
    // return value used to set the current floor the elveator is on
    return mode - 1;
  }

  // when the door is closing
  if (degree == 0)
  {
    // gradually close the door
    for (int i = 90; i >= 0; i -= 5)
    {
      /************ Start US Measurement Section ***********/
      digitalWrite(TRIGGER_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGGER_PIN, LOW);
      duration = pulseIn(ECHO_PIN, HIGH);
      distance = duration * 0.034 / 2;
      /************** End US Measurement Section ***********/

      // if something is blocking the doorway
      if (distance <= 100)
      {
        servo1.write(95); // keeps door open
        return true;      // breaks out of function showing doorway blocked
      }

      servo1.write(i);
      delay(100);
    }
    return false; // indicates doorway not blocked
  }
}
