int joy1x = 0;
int joy1y = 1;

int joy2x = 2;
int joy2y = 3;

int x1Val, y1Val, x2Val, y2Val;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    x1Val = analogRead(joy1x);
    y1Val = analogRead(joy1y);
    x2Val = analogRead(joy2x);
    y2Val = analogRead(joy2y);

    Serial.print("X1: ");
    Serial.print(x1Val);
    Serial.print("  \t| Y1: ");
    Serial.print(y1Val);
    Serial.print("  \t| X2: ");
    Serial.print(x2Val);
    Serial.print("  \t| Y2: ");
    Serial.println(y2Val);

    delay(200);
}