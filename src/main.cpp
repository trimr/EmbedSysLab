#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// const variables, config info
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // used ports of lcd display
const int monitorPort = A5;            // analog port
const float V_in = 5.0;                // voltage at 5V pin of arduino
const int MAXTIME = 10000;             // timeout limitation(millisecond)
const int DELAYTIME = 1000;            // delay time(millisecond)
const float R1 = 1000;                 // known resistance

byte Ohm[8] = {
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b10001,
    0b01010,
    0b11011,
    0b00000};

// other variables used in identification process
float resistanceValue = 0; // placeholder for unknown resistance
int timeoutCounter = 0;

// custom func. to calculate the value of the resistance. also check if the value exceeds the limiation.
float getResistanceValue(float analogValue)
{
  float V_out = analogValue / 1024 * V_in;
  float value = R1 * V_out / (V_in - V_out);
  return value;
}

// custom func. to display info on the lcd display.
void display(int timeoutCounter, float rValue)
{
  lcd.clear();
  if (timeoutCounter == 0)
  {
    lcd.print("IDENTIFICATING..");
    lcd.setCursor(0, 1);
    if (rValue < R1 * 0.1)
    {
      lcd.print("VALUE TOO SMALL!");
    }
    else if (rValue > R1 * 10)
    {
      lcd.print("VALUE TOO BIG!");
    }
    else
    {
      lcd.print("VALUE: ");
      lcd.print(resistanceValue, 2);
      // lcd.print(" Ohm");
      lcd.write(byte(0));
    }
  }
  else
  {
    lcd.print("NO RESISTANCE!");
    lcd.setCursor(0, 1);
    lcd.print("ENDS IN ");
    lcd.print((MAXTIME - timeoutCounter * DELAYTIME) / 1000);
    lcd.print("s!");
  }
}

// setup function
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.createChar(0, Ohm);
}

// loop function
void loop()
{
  int analogValue = analogRead(monitorPort);

  if (timeoutCounter < MAXTIME / DELAYTIME) // if not overtime
  {
    if (analogValue >= 1023) // if no resistance is set
    {
      timeoutCounter++;
    }
    else
    {
      timeoutCounter = 0;
      resistanceValue = getResistanceValue(analogValue);
      Serial.print(resistanceValue);
    }
    Serial.print(", Overtime counter: ");
    Serial.print(timeoutCounter);
    Serial.print("\n");

    display(timeoutCounter, resistanceValue);
  }
  else
  {
    lcd.clear();
    lcd.print("TIMED OUT, ");
    lcd.setCursor(0, 1);
    lcd.print("PROCESS ENDED.");
    Serial.println("TIME OUT!!!");
  }

  // loop delay
  delay(DELAYTIME);
}