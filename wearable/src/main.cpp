#include <Stepper.h>

#define MOTOR_STEPS
Stepper stepper(MOTOR_STEPS, pin1, pin2, pin3, pin4);

int ledGreen = 5;
int ledYellow = 6;
int ledRed = 7;

// Moving average filter parameters
const int numReadings = 10; 
float readings[numReadings]; 
int readIndex = 0; 
float total = 0; 
float average = 0; 

void setup() {
  Serial.begin(9600);
  // Initialize LEDs as outputs
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  stepper.setSpeed(10); 

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

float getUVIndex() {
  // Implementation depends on your communication setup
}

void loop() {
  total = total - readings[readIndex];
  readings[readIndex] = getUVIndex();
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;

  if(average < 3) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
  } else if(average >= 3 && average < 6) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
  } else {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
  }

  // Move the stepper motor according to the averaged UV index
  int steps = calculateStepsForUVIndex(average);
  stepper.step(steps);

  delay(60000); // Delay before next update
}

// Placeholder function to calculate steps for the stepper motor
int calculateStepsForUVIndex(float UVIndex) {
 
}
