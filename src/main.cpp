#include <Arduino.h>

// Function prototypes
void setupCarSemafor(int redPin, int orangePin, int greenPin);
void setupPedestrianSemafor(int pedestrianRedPin, int pedestrianGreenPin, int buttonPin);
void handleNightMode();
void blinkOrangeLight(int orangePin);
void handlePedestrianTrafficLights(unsigned long currentMillis);
void handleCarTrafficLights(unsigned long currentMillis);
void setTrafficLights(int redPin, int orangePin, int greenPin, int redState, int orangeState, int greenState);
void setPedestrianTrafficLights(int redPin, int greenPin, int redState, int greenState);
int chooseActiveCarLight(int color);
int chooseInactiveCarLight(int color);
bool chooseActivePedestrianButtonRequest();
int chooseActivePedestrianLightOrState(int color);
int chooseInactivePedestrianLightOrState(int color);
void switchActiveSemafor();
bool setActivePedestrianButtonRequest(bool pedestrianRequest);

// Car Semafor 1 pins
const int redLightPin1 = 2;
const int orangeLightPin1 = 3;
const int greenLightPin1 = 4;

// Pedestrian Semafor 1 pins
const int pedestrianRedLightPin1 = 5;
const int pedestrianGreenLightPin1 = 6;
const int pedestrianButtonPin1 = 7;

// Car Semafor 2 pins
const int redLightPin2 = 8;
const int orangeLightPin2 = 9;
const int greenLightPin2 = 10;

// Pedestrian Semafor 2 pins
const int pedestrianRedLightPin2 = 11;
const int pedestrianGreenLightPin2 = 12;
const int pedestrianButtonPin2 = 13;

// Night mode button
const int nightModeButtonPin = 14;

const unsigned long redLightDuration = 5000; // 5 seconds
const unsigned long orangeLightDuration = 1000; // 1 second
const unsigned long greenLightDuration = 10000; // 10 seconds
const unsigned long pedestrianRedLightDuration = 15000; // 15 seconds
const unsigned long pedestrianWaitTime = 2000; // 2 seconds

const int RED = 0;
const int RED_AND_ORANGE = 1;
const int GREEN = 2;
const int ORANGE = 3;

const char RED_LIGHT_NAME = 'R';
const char ORANGE_LIGHT_NAME = 'O';
const char GREEN_LIGHT_NAME = 'G';
const char BUTTON_REQUEST_NAME = 'B';

unsigned long previousMillis = 0;
int carLightState = RED;
bool pedestrianRequest1 = false;
bool pedestrianRequest2 = false;
bool nightMode = true; // Initial state is night mode

int activeCarSemafor = 1;
int activePedestrianSemafor = 2;

void setup() {
  Serial.begin(9600);
  
  setupCarSemafor(redLightPin1, orangeLightPin1, greenLightPin1);
  setupCarSemafor(redLightPin2, orangeLightPin2, greenLightPin2);
  
  setupPedestrianSemafor(pedestrianRedLightPin1, pedestrianGreenLightPin1, pedestrianButtonPin1);
  setupPedestrianSemafor(pedestrianRedLightPin2, pedestrianGreenLightPin2, pedestrianButtonPin2);
  
  pinMode(nightModeButtonPin, INPUT_PULLUP);
}

void loop() {
  unsigned long currentMillis = millis();
  int nightModeButtonState = digitalRead(nightModeButtonPin);

  if (nightModeButtonState == LOW) {
    nightMode = !nightMode;
    delay(500); // Debounce delay
    Serial.println(nightMode ? "Night mode activated" : "Night mode deactivated");
  }

  if (nightMode) {
    handleNightMode();
    return;
  }

  handleCarTrafficLights(currentMillis);
  handlePedestrianTrafficLights(currentMillis);
}

void setupCarSemafor(int redPin, int orangePin, int greenPin) {
  pinMode(redPin, OUTPUT);
  pinMode(orangePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void setupPedestrianSemafor(int pedestrianRedPin, int pedestrianGreenPin, int buttonPin) {
  pinMode(pedestrianRedPin, OUTPUT);
  pinMode(pedestrianGreenPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void handleNightMode() {
  blinkOrangeLight(orangeLightPin1);
  blinkOrangeLight(orangeLightPin2);
  digitalWrite(pedestrianRedLightPin1, LOW);
  digitalWrite(pedestrianGreenLightPin1, LOW);
  digitalWrite(pedestrianRedLightPin2, LOW);
  digitalWrite(pedestrianGreenLightPin2, LOW);
}

void blinkOrangeLight(int orangePin) {
  digitalWrite(orangePin, millis() % 1000 < 500 ? HIGH : LOW);
}

void handlePedestrianTrafficLights(unsigned long currentMillis) {
  int activeRedLight = chooseInactivePedestrianLightOrState(RED_LIGHT_NAME);
  int activeGreenLight = chooseInactivePedestrianLightOrState(GREEN_LIGHT_NAME);
  int activeButtonPin = chooseInactivePedestrianLightOrState(BUTTON_REQUEST_NAME);
  bool pedestrianButtonRequest = chooseActivePedestrianButtonRequest();

  int activeButtonState = digitalRead(activeButtonPin);

  int inactiveRedLight = chooseInactivePedestrianLightOrState(RED_LIGHT_NAME);
  int inactiveGreenLight = chooseInactivePedestrianLightOrState(GREEN_LIGHT_NAME);

  if (activeButtonState == LOW && !pedestrianButtonRequest) {
    pedestrianButtonRequest = setActivePedestrianButtonRequest(true);
    previousMillis = currentMillis;
    delay(500); // Debounce delay
    Serial.println("Pedestrian button pressed for Pedestrian Semafor ");
    Serial.println(activePedestrianSemafor);
  }
  
  if(pedestrianButtonRequest) {
    if (currentMillis - previousMillis >= pedestrianWaitTime) { // Wait 2 seconds
      setPedestrianTrafficLights(activeRedLight, activeGreenLight, LOW, HIGH);
      Serial.println("Pedestrian light green");
      if (currentMillis - previousMillis >= pedestrianWaitTime + pedestrianRedLightDuration) {
        pedestrianButtonRequest = setActivePedestrianButtonRequest(false);
        previousMillis = currentMillis;
        setPedestrianTrafficLights(activeRedLight, activeGreenLight, HIGH, LOW);
        Serial.println("Pedestrian light red");
      }
    }
  } else {
    setPedestrianTrafficLights(activeRedLight, activeGreenLight, HIGH, LOW);
    setPedestrianTrafficLights(inactiveRedLight, inactiveGreenLight, HIGH, LOW);
  }
}

void handleCarTrafficLights(unsigned long currentMillis) {
  int activeRedLight = chooseActiveCarLight(RED_LIGHT_NAME);
  int activeOrangeLight = chooseActiveCarLight(ORANGE_LIGHT_NAME);
  int activeGreenLight = chooseActiveCarLight(GREEN_LIGHT_NAME);

  int inactiveRedLight = chooseInactiveCarLight(RED_LIGHT_NAME);
  int inactiveOrangeLight = chooseInactiveCarLight(ORANGE_LIGHT_NAME);
  int inactiveGreenLight = chooseInactiveCarLight(GREEN_LIGHT_NAME);
  
  switch (carLightState) {
    case RED: // Car Semafor 1: Red, Car Semafor 2: Green
      if (currentMillis - previousMillis >= redLightDuration) {
        previousMillis = currentMillis;
        carLightState = RED_AND_ORANGE;
        Serial.print("Transition to RED_AND_ORANGE for Car Semafor ");
        Serial.println(activeCarSemafor);
      }
      setTrafficLights(activeRedLight, activeOrangeLight, activeGreenLight, HIGH, LOW, LOW);
      setTrafficLights(inactiveRedLight, inactiveOrangeLight, inactiveGreenLight, LOW, LOW, HIGH);
      break;
    case RED_AND_ORANGE: // Car Semafor 1: Red and Orange, Car Semafor 2: Green
      if (currentMillis - previousMillis >= orangeLightDuration) {
        previousMillis = currentMillis;
        carLightState = GREEN;
        Serial.print("Transition to GREEN for Car Semafor ");
        Serial.println(activeCarSemafor);
      }
      setTrafficLights(activeRedLight, activeOrangeLight, activeGreenLight, HIGH, HIGH, LOW);
      setTrafficLights(inactiveRedLight, inactiveOrangeLight, inactiveGreenLight, LOW, LOW, HIGH);
      break;
    case GREEN: // Car Semafor 1: Green, Car Semafor 2: Red
      if (currentMillis - previousMillis >= greenLightDuration) {
        previousMillis = currentMillis;
        carLightState = ORANGE;
        Serial.print("Transition to ORANGE for Car Semafor ");
        Serial.println(activeCarSemafor);
      }
      setTrafficLights(activeRedLight, activeOrangeLight, activeGreenLight, LOW, LOW, HIGH);
      setTrafficLights(inactiveRedLight, inactiveOrangeLight, inactiveGreenLight, HIGH, LOW, LOW);
      break;
    case ORANGE: // Car Semafor 1: Orange, Car Semafor 2: Red
      if (currentMillis - previousMillis >= orangeLightDuration) {
        previousMillis = currentMillis;
        carLightState = RED;
        Serial.print("Transition to RED for Car Semafor ");
        Serial.println(activeCarSemafor);

        switchActiveSemafor();
      }
      setTrafficLights(activeRedLight, activeOrangeLight, activeGreenLight, LOW, HIGH, LOW);
      setTrafficLights(inactiveRedLight, inactiveOrangeLight, inactiveGreenLight, HIGH, LOW, LOW);
      break;
  }
}

void setTrafficLights(int redPin, int orangePin, int greenPin, int redState, int orangeState, int greenState) {
  digitalWrite(redPin, redState);
  digitalWrite(orangePin, orangeState);
  digitalWrite(greenPin, greenState);
}

void setPedestrianTrafficLights(int redPin, int greenPin, int redState, int greenState) {
  digitalWrite(redPin, redState);
  digitalWrite(greenPin, greenState);
}

int chooseActiveCarLight(int color) {
  if (activeCarSemafor == 1) {
    if (color == RED_LIGHT_NAME) return redLightPin1;
    if (color == ORANGE_LIGHT_NAME) return orangeLightPin1;
    if (color == GREEN_LIGHT_NAME) return greenLightPin1;
  } else if (activeCarSemafor == 2) {
    if (color == RED_LIGHT_NAME) return redLightPin2;
    if (color == ORANGE_LIGHT_NAME) return orangeLightPin2;
    if (color == GREEN_LIGHT_NAME) return greenLightPin2;
  }
  return -1; // Invalid input
}

int chooseInactiveCarLight(int color) {
  if (activeCarSemafor == 1) {
    if (color == RED_LIGHT_NAME) return redLightPin2;
    if (color == ORANGE_LIGHT_NAME) return orangeLightPin2;
    if (color == GREEN_LIGHT_NAME) return greenLightPin2;
  } else if (activeCarSemafor == 2) {
    if (color == RED_LIGHT_NAME) return redLightPin1;
    if (color == ORANGE_LIGHT_NAME) return orangeLightPin1;
    if (color == GREEN_LIGHT_NAME) return greenLightPin1;
  }
  return -1; // Invalid input
}

bool chooseActivePedestrianButtonRequest() {
  if (activePedestrianSemafor == 1) {
    return pedestrianRequest1;
  } else if (activePedestrianSemafor == 2) {
    return pedestrianRequest2;
  }
  return false;
}

int chooseActivePedestrianLightOrState(int color) {
  if (activePedestrianSemafor == 1) {
    if (color == RED_LIGHT_NAME) return pedestrianRedLightPin1;
    if (color == GREEN_LIGHT_NAME) return pedestrianGreenLightPin1;
    if (color == BUTTON_REQUEST_NAME) return pedestrianRequest1;
  } else if (activePedestrianSemafor == 2) {
    if (color == RED_LIGHT_NAME) return pedestrianRedLightPin2;
    if (color == GREEN_LIGHT_NAME) return pedestrianGreenLightPin2;
    if (color == BUTTON_REQUEST_NAME) return pedestrianRequest2;
  }
  return -1; // Invalid input
}

int chooseInactivePedestrianLightOrState(int color) {
  if (activePedestrianSemafor == 1) {
    if (color == RED_LIGHT_NAME) return pedestrianRedLightPin2;
    if (color == GREEN_LIGHT_NAME) return pedestrianGreenLightPin2;
    if (color == BUTTON_REQUEST_NAME) return pedestrianRequest2;
  } else if (activePedestrianSemafor == 2) {
    if (color == RED_LIGHT_NAME) return pedestrianRedLightPin1;
    if (color == GREEN_LIGHT_NAME) return pedestrianGreenLightPin1;
    if (color == BUTTON_REQUEST_NAME) return pedestrianRequest1;
  }
  return -1; // Invalid input
}

void switchActiveSemafor() {
  activeCarSemafor = activeCarSemafor == 1 ? 2 : 1;
  Serial.print("Switched active Car Semafor to ");
  Serial.println(activeCarSemafor);

  activePedestrianSemafor = activePedestrianSemafor == 1 ? 2 : 1;
  Serial.print("Switched active Pedestrian Semafor to ");
  Serial.println(activePedestrianSemafor);
}

bool setActivePedestrianButtonRequest(bool pedestrianRequest) {
  if (activePedestrianSemafor == 1) {
    pedestrianRequest1 = pedestrianRequest;
    return pedestrianRequest1;
  } else if (activePedestrianSemafor == 2) {
    pedestrianRequest2 = pedestrianRequest;
    return pedestrianRequest2;
  }

  return false;
}