#include <Stepper.h>
#include "BluetoothSerial.h"
 
BluetoothSerial SerialBT;

// Stepper motor setup
#define MOTOR_STEPS 2048
#define MOTOR_SPEED 15
Stepper stepper(MOTOR_STEPS, motorPin1, motorPin2, motorPin3, motorPin4);

// LED pins
int ledGreen = 5;
int ledYellow = 6;
int ledRed = 7;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("UV Display"); 
  
  // Initialize LEDs
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  // Initialize stepper motor
  stepper.setSpeed(MOTOR_SPEED);
}

void loop() {
  if (SerialBT.available()) {
    float UVindex = SerialBT.parseFloat();
    if (SerialBT.read() == '\n') {
      updateDisplay(UVindex);
    }
  }
}

void updateDisplay(float UVindex) {
  int angle = getAngleForUVLevel(UVindex); // Get angle for UV level
  stepper.step(angle); // Rotate stepper motor to the angle
  
  // Update LEDs based on UV level
  if(UVindex < 3) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
  } else if(UVindex >= 3 && UVindex <= 6) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
  } else {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
  }
}

int getAngleForUVLevel(float UVindex) {
  int stepPerRevolution = MOTOR_STEPS / 8; 
  int angleStep = stepPerRevolution * ((int)UVindex - 1);
  return angleStep % MOTOR_STEPS; 
}
