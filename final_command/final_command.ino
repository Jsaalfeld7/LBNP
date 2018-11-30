#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x3F // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
void lcdPrint(int c,int r,String s);

int timeout1;
int timeout2;
int p;
int pPrev;
int t;
int tPrev;
int h;
int hPrev;
int checkData();

void setup() {
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  Serial.begin(9600);
  lcd.begin(20,4);
  lcdPrint(0,0,"mmHg: ");
  lcdPrint(0,1,"Temp (C): ");
  lcdPrint(0,2,"Humidity (%): ");
}

void loop() {
  if(Serial.available() == 0 and timeout1 == 10){
    lcdPrint(6,0,"Searching for");
    lcdPrint(10,1,"P/T signal");
    lcdPrint(14,2,"Search");
    pPrev = 999;
    tPrev = 999;
    Serial.flush();
  }
  else if(Serial.available() == 0 and timeout1 < 10){
    timeout1++;
    pPrev = 999;
    tPrev = 999;
    Serial.flush();
  }
  else if(Serial.available() > 0){
    for(int ii = 0;ii <=Serial.available()-1;ii++){
      int check = checkData();
      switch(check){
        case 1:
          if(p>-75 and p<75){
            lcdPrint(6,0,String(p*-1));
          }
          break;
        case 2:
          if(t>0 and t<100){
            lcdPrint(10,1,String(t));
          }
          break;
        case 3:
          if(h>0 and h<100){
            lcdPrint(14,2,String(h));
          }
          break;
      }
    }
    timeout1 = 0;
  }
  delay(200);
}

void lcdPrint(int c, int r, String s){
  lcd.setCursor(c,r);
  for(int i = 0; i < 20-c; i++){
    lcd.print(" "); //clears the rest of the line
  }
  lcd.setCursor(c,r);
  lcd.print(s);
  return;
}

int checkData(){
  int state;
  int temp;
  pPrev = p;
  tPrev = t;
  hPrev = h;
  temp = Serial.read();
  if(temp == 1){
    temp = 10;
  }
  String s = String(temp);
  if(s[0] == '1'){
    s.remove(0,1);
    p = s.toInt();
    state = 1;
  }
  else if(s[0] == '2'){
    s.remove(0,1);
    t = s.toInt();
    state = 2;
  }
  else if(s[0] == '3'){
    s.remove(0,1);
    h = Serial.read();
    state = 3;
  }
  return state;
}

