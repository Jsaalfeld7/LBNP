#include <Wire.h>
#include "cactus_io_BME280_I2C.h"

// Create BME280 object
Adafruit_BME280 bme;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Bosch BME280 Barometric Pressure - Humidity - Temp Sensor");
  
  int t
  int p
  int h
  
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);

   }
  bme.setTempCal(-1);

  Serial.println("Pressure\Humidity\Temperature");

}

void loop() {
  // put your main code here, to run repeatedly:

bme.readSensor();

Serial.print(bme.getPressure_MB()); Serial.print("\t\t"); // Pressure in millibars
Serial.print(bme.getHumidity()); Serial.print("\t\t");
Serial.print(bme.getTemperature_C()); Serial.print(" *C\t");

delay(2000);
}
