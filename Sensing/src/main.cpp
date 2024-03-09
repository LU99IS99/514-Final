#include <Wire.h>
#include "Adafruit_SI1145.h"
#include "BluetoothSerial.h"

Adafruit_SI1145 uv = Adafruit_SI1145();
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("UV Sensor"); 
  
  if (!uv.begin()) {
    Serial.println("Didn't find Si1133");
    while (1);
  }
  Serial.println("Si1133 is ready!");
}

void loop() {
  float UVindex = uv.readUV() / 100.0; 
  SerialBT.println(UVindex); 
  delay(60000); // Send data every 60 seconds
}
