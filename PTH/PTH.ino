#include <Wire.h>
#include "cactus_io_DHT22.h"

int h;
int count = 6;
int p; // pressure to be sent
int t; // temperature to be sent
int pVar;
int tVar;
int addr = 0x28; // address of I2C device
int d1, d2, d3, d4; // 4 bits of data being transferred
bool conn; // checks for connection with I2C device

#define DHT22_PIN 2
DHT22 dht(DHT22_PIN);

void readData();
String intToBin(int x, int sig);
String bytesBit(String s, int x);
int binToInt(String str);
void markP(int x);
void markT(int x);
void markH(int x);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  if (count == 6) {
    dht.readHumidity();
    dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(dht.humidity) || isnan(dht.temperature_C)) {
      //Serial.println("DHT sensor read failure!");
      return;
    }
    h = dht.humidity;
    markH(h);
    count = 0;
    Serial.write(3);
    Serial.write(h);
  }
  else {
    readData();
    if (conn) {
      String s1, s2, s3, s4;
      s1 = intToBin(d1, 8);
      s2 = intToBin(d2, 8);
      s3 = intToBin(d3, 8);
      s4 = intToBin(d4, 8);
      String data1, data2;
      data1 = s1 + s2;
      data2 = s3 + s4;
      data1 = bytesBit(data1, 1);
      data2 = bytesBit(data2, 2);
      pVar = binToInt(data1);
      tVar = binToInt(data2);
      p = ((pVar - 1638.) / (65.535) - 100.) / -1.33322; // convert pressure to mmHg
      t = (tVar * 200.) / 2048. - 50; // conversion to celsius
      markP(p);
      markT(t);
      Serial.write(p);
      Serial.write(t);
    }
    count++;
  }
  delay(500);
}

void readData() {
  Wire.beginTransmission(addr);
  Wire.endTransmission(addr);
  Wire.requestFrom(addr, 4);
  if (Wire.available() == 4) {
    d1 = Wire.read(); // read in first byte from sensorw
    d2 = Wire.read(); // read in second byte from sensor
    d3 = Wire.read(); // read in third byte from sensor
    d4 = Wire.read(); // read in fourth byte from sensor
    conn = true;
  }
  else {
    conn = false;
  }
}

String intToBin(int x, int sig) {
  int temp = x;
  int remainder;
  String bin = "";
  if (temp == 0) {
    return "0";
  }
  else {
    for (int ii = sig - 1; ii >= 0; ii--) {
      remainder = temp - pow(2, ii);
      if (remainder < 0) {
        bin = bin + "0";
      }
      else {
        bin = bin + "1";
        temp = remainder;
      }
    }
  }
  return bin;
}

String bytesBit(String s, int x) {
  switch (x) {
    case 1:
      s.remove(0, 2);
      break;
    case 2:
      s.remove(11);
      break;
  }
  return s;
}

int binToInt(String str) {
  int temp = 0;
  int len = str.length() - 1;
  for (int ii = 0; ii < len; ii++) {
    if (str[ii] == '1') {
      temp = temp + int(pow(2, len - ii));
    }
  }
  return temp;
}

void markP(int x) {
  String temp = String(x);
  temp = "1" + temp;
  p = temp.toInt();
}

void markT(int x) {
  String temp = String(x);
  temp = "2" + temp;
  t = temp.toInt();
}

void markH(int x) {
  String temp = String(x);
  temp = "0" + temp;
  h = temp.toInt();
}
