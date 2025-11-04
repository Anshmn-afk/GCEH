#include <Servo.h>

Servo finger[6];   // 0–4 fingers, 5 = wrist
int servoPins[6] = {3, 5, 6, 9, 10, 11};  // adjust as wired

// fixed angles for demo
int openPose[6]  = {10, 10, 10, 10, 10, 90};
int fistPose[6]  = {160, 160, 160, 160, 160, 90};
int pointPose[6] = {10, 160, 160, 160, 160, 90};
int thumbsPose[6]= {160, 10, 10, 10, 10, 90};

String cmd;

void setup() {
  Serial.begin(9600);
  for (int i=0;i<6;i++){ finger[i].attach(servoPins[i]); }
  openHand();
  Serial.println("Ready");
}

void loop() {
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "OPEN")  moveServos(openPose);
    else if (cmd == "FIST")  moveServos(fistPose);
    else if (cmd == "POINT") moveServos(pointPose);
    else if (cmd == "THUMB") moveServos(thumbsPose);

    Serial.println("ACK:" + cmd);
  }
}

void moveServos(int pose[6]) {
  for (int i=0;i<6;i++){ finger[i].write(pose[i]); }
  delay(300);
}

void openHand() { moveServos(openPose); }
