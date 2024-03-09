#include <Wire.h>
#include "Adafruit_SI1145.h"
#include "BluetoothSerial.h"

Adafruit_SI1145 uv = Adafruit_SI1145();
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("UV Sensor"); // Start Bluetooth with device name "UV Sensor"
  
  if (!uv.begin()) {
    Serial.println("Didn't find Si1133");
    while (1);
  }
  Serial.println("Si1133 is ready!");
}

void loop() {
  float UVindex = uv.readUV() / 100.0; // Convert to proper UV index value
  SerialBT.println(UVindex); // Send UV index over Bluetooth
  delay(60000); // Send data every 60 seconds
}
