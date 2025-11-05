#include <Servo.h>

Servo thumbServo;
Servo indexServo;
Servo middleServo;
Servo ringServo;
Servo wristServo;

String cmd;

// Define servo pins
int servoPins[5] = {3, 5, 6, 9, 10};

// Define open and close angles (tune these for your mechanism)
int openPose[5] = {10, 10, 10, 10, 90};      // All fingers open
int closePose[5] = {150, 150, 150, 150, 90}; // All fingers closed (fist)

void setup() {
  Serial.begin(9600);

  // Attach servos to pins
  thumbServo.attach(servoPins[0]);
  indexServo.attach(servoPins[1]);
  middleServo.attach(servoPins[2]);
  ringServo.attach(servoPins[3]);
  wristServo.attach(servoPins[4]);

  moveServos(openPose);
  Serial.println("Exo-hand system ready");
}

void loop() {
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "OPEN") {
      moveServos(openPose);
      Serial.println("ACK:OPEN");
    } 
    else if (cmd == "FIST") {
      moveServos(closePose);
      Serial.println("ACK:FIST");
    }
    else if (cmd == "RELAX") {
      relaxServos();
      Serial.println("ACK:RELAX");
    }
  }
}

void moveServos(int pose[5]) {
  thumbServo.write(pose[0]);
  indexServo.write(pose[1]);
  middleServo.write(pose[2]);
  ringServo.write(pose[3]);
  wristServo.write(pose[4]);
  delay(500);
}

void relaxServos() {
  thumbServo.detach();
  indexServo.detach();
  middleServo.detach();
  ringServo.detach();
  wristServo.detach();
}
