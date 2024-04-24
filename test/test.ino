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
const int baseSpeed = 255;
const int turnSpeed = 255;

// Initial turn variables
const int turnAngleDegrees = 179;
int timeSpent = 0;

bool leftTurn = false;



void setup() {

  int stateChange = 0;
  bool change = true;
  bool change2 = true;
  int startTime = millis();

  while (true) {

    int sensorStatus = lineFollower.readSensors();

    turnLeft(turnSpeed);

    if (sensorStatus == 3 && change2 == true) {
      change = true;
      change2 = false;
      stateChange++;
    }
    else if (sensorStatus == 0 && change == true) {
      change = false;
      change2 = true;
      stateChange++;
    }

    if (stateChange >= 5) {
      timeSpent = millis() - startTime;
      stopMoving(); 
      break;
    }

  }

}



void loop() {

  int sensorStatus = lineFollower.readSensors();

  switch(sensorStatus) {

    case 3:
      moveForward(maxSpeed);
      leftTurn = false;
      break;

    case 2:
      leftTurn = true;
      turnLeft(baseSpeed);
      break;
      
    case 1:
      leftTurn = false;
      turnRight(baseSpeed);
      break;

    case 0:
      findLine(turnSpeed);
      leftTurn = false;
      break;

    default:
      break;

  }

}



void findLine(int speed) {

  int turnTime = timeSpent / 360 * turnAngleDegrees;
  int startTime = millis();
  int curTime = millis();

  int sensorStatus = lineFollower.readSensors();

  stopMoving();

  while (curTime - startTime < turnTime && sensorStatus != 3) {

    if (leftTurn) {
      turnLeft(speed);
    }
    else {
      turnRight(speed);
    }

    curTime = millis();
    sensorStatus = lineFollower.readSensors();

  }

  stopMoving();

  while(sensorStatus != 3) {

    if (leftTurn) {
      turnRight(speed);
    }
    else {
      turnLeft(speed);
    }

    sensorStatus = lineFollower.readSensors();

  }

  stopMoving();

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
