#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);   //rs,e,d4,d5,d6,d7
int Vin=5;        //voltage at 5V pin of arduino
float Vout=0;     //voltage at A5 pin of arduino
float R1=220;    //value of known resistance
float R2=0;       //value of unknown resistance
int a2d_data=0;    
float buffer=0;            

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  a2d_data=analogRead(A5);
  if(a2d_data)
  {
    buffer=a2d_data*Vin;
    Vout=(buffer)/1024.0;
    buffer=Vout/(Vin-Vout); 
    R2=R1*buffer;
    lcd.setCursor(4,0);
    lcd.print("ohm meter");
    lcd.setCursor(0,1);
    lcd.print("R (ohm) = ");
    lcd.print(R2);
    
    delay(1000);
  }
}
