#include <Wire.h>
#include "Adafruit_SI1145.h"

Adafruit_SI1145 uv = Adafruit_SI1145();

void setup() {
  Serial.begin(9600);
  if (!uv.begin()) {
    Serial.println("Didn't find Si1133");
    while (1);
  }
  Serial.println("Si1133 UV sensor ready!");
}

void loop() {
  float UVindex = uv.readUV();
  // The UV index reading from the sensor needs to be divided by 100 to get the actual value
  UVindex /= 100.0;  
  Serial.print("UV Index: ");
  Serial.println(UVindex);
  delay(2000); // Delay between reads
}
