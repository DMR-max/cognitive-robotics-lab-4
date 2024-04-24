#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeLineFollower.h>

// Declaring the motor fields
MeDCMotor rightMotor(M1); // Find one motor in slot 9 and call it rightMotor
MeDCMotor leftMotor(M2); // Find another motor in slot 10 and call it leftMotor

// Line follower sensor object
MeLineFollower lineFollower(PORT_2);

// Motor speed constant
const int motorSpeed = 180;
const int turnSpeed = 160;
unsigned long myTime;
bool run = true;
bool change = true;
bool change2 = true;
int honderdtachtig = 0;
int timespent = 0;



void setup() {
  Serial.begin(9600);
  myTime = millis();
  // In deze loop wordt vijf keer van state verandert
  // 1. van start naar wit, 2. wit naar zwart 3. zwart naar wit 4. wit naar zwart 5. zwart naar wit volledige 360.
  while (run == true){
    int sensorStatus = lineFollower.readSensors();
    turnLeft();
    // Start op wit met honderdtachtig = 1
    if (sensorStatus == 3 && change2 == true){
      change2 = false;
      change = true;
      honderdtachtig = honderdtachtig + 1;
    }
    // na start zwart dus honderdtachtig = 2
    if (sensorStatus == 0 && change == true){
        honderdtachtig = honderdtachtig + 1;
        change = false;
        change2 = true;
        Serial.println(sensorStatus);
    }
    if (honderdtachtig == 5){
      timespent = millis() - myTime;
      run = false; 
      stopMoving();     
    }

  }

}



void loop() {

//   int sensorStatus = lineFollower.readSensors();
//   Serial.println(sensorStatus);

//   // Value:   left sensor:   right sensor: 
//   //   0        black           black
//   //   1        black           white
//   //   2        white           black
//   //   3        white           white

//   if (sensorStatus == 3) {
//     moveForward();
//   }
//   else if (sensorStatus == 2) {
//     turnLeft();
//   }
//   else if (sensorStatus == 1) {
//     turnRight();
//   }
//   else if (sensorStatus == 0) {
//     findLine();
//   }
//   else {
//     stopMoving();
//   }

}



void findLine() {

  leftMotor.stop();
  rightMotor.stop();

  while (lineFollower.readSensors() != 3) {
    leftMotor.run(turnSpeed);
    rightMotor.run(turnSpeed);
  }

  leftMotor.stop();
  rightMotor.stop();

}



void moveForward() {
  leftMotor.run(-motorSpeed);
  rightMotor.run(motorSpeed);
}



void turnLeft() {
  leftMotor.run(motorSpeed);
  rightMotor.run(motorSpeed);
}



void turnRight() {
  leftMotor.run(-motorSpeed);
  rightMotor.run(-motorSpeed);
}



void stopMoving() {
  leftMotor.stop();
  rightMotor.stop();
}

