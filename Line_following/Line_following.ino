#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeLineFollower.h>

// Value:   left sensor:   right sensor: 
//   0        black           black
//   1        black           white
//   2        white           black
//   3        white           white

// Motors and sensors
MeDCMotor leftMotor(M2);
MeDCMotor rightMotor(M1);
MeLineFollower lineFollower(PORT_2);

// Speed variables
const int maxSpeed = 255;
const int baseSpeed = 200;
const int turnSpeed = 160;

// Initial turn variables
const int turnAngleDegrees = 110;
int timeSpent = 0;


void setup() {

  int startTime = millis();
  int stateChange = 0;
  bool run = true;
  bool change = true;
  bool change2 = true;

  while (run == true) {

    int sensorStatus = lineFollower.readSensors();

    turnLeft(turnSpeed);

    if (sensorStatus == 3 && change2 == true) {
      change2 = false;
      change = true;
      stateChange++;
    }
    else if (sensorStatus == 0 && change == true) {
        stateChange++;
        change = false;
        change2 = true;
    }

    if (stateChange == 5) {
      timeSpent = millis() - startTime;
      run = false; 
      stopMoving(); 
    }

  }

}



void loop() {

  int sensorStatus = lineFollower.readSensors();

  switch(sensorStatus) {

    case 3:
      moveForward(maxSpeed);
      break;

    case 2:
      turnLeft(baseSpeed);
      break;
      
    case 1:
      turnRight(baseSpeed);
      break;

    case 0:
      findLine(turnSpeed);
      break;

    default:
      stopMoving();
      break;

  }

}



void findLine(int speed) {

  int startTime = millis();
  float turnTime = timeSpent / 360 * turnAngleDegrees;

  stopMoving();

  while (millis() - startTime < turnTime && lineFollower.readSensors() == 0) {
    turnLeft(speed);
  }

  stopMoving();

  if (lineFollower.readSensors() == 0) {
    while(lineFollower.readSensors() == 0) {
      turnRight(speed);
    }
    stopMoving();
  }

}



void moveForward(int speed) {
  leftMotor.run(-speed);
  rightMotor.run(speed);
}



void turnLeft(int speed) {
  leftMotor.run(speed);
  rightMotor.run(speed);
}



void turnRight(int speed) {
  leftMotor.run(-speed);
  rightMotor.run(-speed);
}



void stopMoving() {
  leftMotor.stop();
  rightMotor.stop();
}
