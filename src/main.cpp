#include <Arduino.h>

const int redLightPin = 2;
const int orangeLightPin = 3;
const int greenLightPin = 4;
const int buttonPin = 5;
const int pedestrianLightPin = 6; // New pin for pedestrian crossing light

const unsigned long redLightDuration = 5000; // 5 seconds
const unsigned long orangeLightDuration = 1000; // 1 second
const unsigned long greenLightDuration = 10000; // 10 seconds
const unsigned long pedestrianRedLightDuration = 15000; // 15 seconds

unsigned long previousMillis = 0;
int lightState = 0; // 0: Red, 1: Red and Orange, 2: Green, 3: Orange
bool pedestrianRequest = false;

void setup() {
  pinMode(redLightPin, OUTPUT);
  pinMode(orangeLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pedestrianLightPin, OUTPUT); // Set pedestrian light pin as output
}

void loop() {
  unsigned long currentMillis = millis();
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && !pedestrianRequest) {
    // Pedestrian button pressed, set request flag
    pedestrianRequest = true;
    previousMillis = currentMillis;
  }

  if (pedestrianRequest) {
    if (currentMillis - previousMillis >= 2000) { // Wait 2 seconds
      // Pedestrian crossing, show red light for 15 seconds
      digitalWrite(redLightPin, HIGH);
      digitalWrite(orangeLightPin, LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(pedestrianLightPin, HIGH); // Turn on pedestrian light
      if (currentMillis - previousMillis >= 2000 + pedestrianRedLightDuration) {
        // Reset after pedestrian crossing
        pedestrianRequest = false;
        previousMillis = currentMillis;
        lightState = 0; // Reset to red light
        digitalWrite(pedestrianLightPin, LOW); // Turn off pedestrian light
      }
    }
  } else {
    // Normal traffic light cycle
    switch (lightState) {
      case 0: // Red light
        if (currentMillis - previousMillis >= redLightDuration) {
          previousMillis = currentMillis;
          lightState = 1; // Transition to Red and Orange
        }
        digitalWrite(redLightPin, HIGH);
        digitalWrite(orangeLightPin, LOW);
        digitalWrite(greenLightPin, LOW);
        break;
      case 1: // Red and Orange
        if (currentMillis - previousMillis >= orangeLightDuration) {
          previousMillis = currentMillis;
          lightState = 2; // Transition to Green
        }
        digitalWrite(redLightPin, HIGH);
        digitalWrite(orangeLightPin, HIGH);
        digitalWrite(greenLightPin, LOW);
        break;
      case 2: // Green light
        if (currentMillis - previousMillis >= greenLightDuration) {
          previousMillis = currentMillis;
          lightState = 3; // Transition to Orange
        }
        digitalWrite(redLightPin, LOW);
        digitalWrite(orangeLightPin, LOW);
        digitalWrite(greenLightPin, HIGH);
        break;
      case 3: // Orange light
        if (currentMillis - previousMillis >= orangeLightDuration) {
          previousMillis = currentMillis;
          lightState = 0; // Transition to Red
        }
        digitalWrite(redLightPin, LOW);
        digitalWrite(orangeLightPin, HIGH);
        digitalWrite(greenLightPin, LOW);
        break;
    }
  }
}