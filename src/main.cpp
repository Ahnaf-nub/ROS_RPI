#include <Servo.h>
#include <NewPing.h>

#define trigPin 3
#define echoPin 2
#define servoPin 0

Servo servo; // Create servo object
NewPing sonar(trigPin, echoPin, 200); // NewPing setup of pins and maximum distance

void setup() {
  Serial.begin(9600); // Start serial communication for debugging

  // Attach the servo
  servo.attach(servoPin);

  // Start with the servo at 0 degrees
  servo.write(0);
}

float getDistance() {
  float distance = sonar.ping_cm();
  return distance;
}


void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 10) {
    servo.write(angle);   // Move servo to current angle
    delay(200);           // Wait for the servo to reach the position
    
    // Measure distance with sonar
    float distance = getDistance();
    
    // Print the angle and distance
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°, Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // Sweep the servo back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 10) {
    servo.write(angle);
    delay(200);

    float distance = getDistance();

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°, Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}