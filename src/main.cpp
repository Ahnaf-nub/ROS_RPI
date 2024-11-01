#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Motor A control pins
const int IN1_A = 28;  // Motor A IN1 pin
const int IN2_A = 27;  // Motor A IN2 pin
const int ENA_A = 20;  // Motor A enable pin (PWM)

// Motor B control pins
const int IN1_B = 21;  // Motor B IN1 pin
const int IN2_B = 22;  // Motor B IN2 pin
const int ENA_B = 19;  // Motor B enable pin (PWM)

String command;   // Serial command input
int speed = 200;  // Default speed (0-255)

// Animation functions for display feedback
void animateForward() {
  Serial.println("Moving Forward...");
  delay(1000);
}

void animateBackward() {
  Serial.println("Moving Backward...");
  delay(1000);
}

void animateLeft() {
  Serial.println("Turning Left...");
  delay(1000);
}

void animateRight() {
  Serial.println("Turning Right...");
  delay(1000);
}

void animateStop() {
  Serial.println("Stopping...");
  delay(1000);
}

// Motor control functions
void setMotorA(bool forward, int speed) {
  digitalWrite(IN1_A, forward);
  digitalWrite(IN2_A, !forward);
  analogWrite(ENA_A, speed);
}

void setMotorB(bool forward, int speed) {
  digitalWrite(IN1_B, forward);
  digitalWrite(IN2_B, !forward);
  analogWrite(ENA_B, speed);
}

void moveForward() {
  setMotorA(true, speed);
  setMotorB(true, speed);
  animateForward();
}

void moveBackward() {
  setMotorA(false, speed);
  setMotorB(false, speed);
  animateBackward();
}

void turnLeft() {
  setMotorA(false, speed);
  setMotorB(true, speed);
  animateLeft();
}

void turnRight() {
  setMotorA(true, speed);
  setMotorB(false, speed);
  animateRight();
}

void stopMotors() {
  setMotorA(true, 0);  // Setting speed to 0 stops the motor
  setMotorB(true, 0);
  animateStop();
}

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(ENA_A, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_B, OUTPUT);
  pinMode(ENA_B, OUTPUT);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.println("TFT Display Initialized");
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    command.trim(); // Remove any extra whitespace

    if (command == "FORWARD") {
      moveForward();
    } 
    else if (command == "BACKWARD") {
      moveBackward();
    } 
    else if (command == "LEFT") {
      turnLeft();
    } 
    else if (command == "RIGHT") {
      turnRight();
    } 
    else if (command == "STOP") {
      stopMotors();
    } 
    else if (command.startsWith("SPEED ")) {
      int newSpeed = command.substring(6).toInt();
      speed = constrain(newSpeed, 0, 255);
      Serial.print("Speed set to: ");
      Serial.println(speed);
    } else {
      Serial.println("Invalid command. Available commands: FORWARD, BACKWARD, LEFT, RIGHT, STOP, SPEED <value>");
    }
  }
}