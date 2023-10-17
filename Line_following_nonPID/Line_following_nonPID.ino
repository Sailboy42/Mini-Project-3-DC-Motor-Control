#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Setting up Motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);   // we are using M3 port
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);  // we are using M1 port

//sensor pins
#define leftSensor A2
#define centerSensor A1
#define rightSensor A0

//Motor pins
#define L_MOTOR 6
#define R_MOTOR 7
#define threshold 850
#define motor_speed 30

int lastAction = 0;

//Motor variables
int motorSpeed = 25;
int speedReverse = -1;
int speedLeft = 25;
int speedRight = 25;

bool start = true;  //Start condition

//Serial control setup
const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data
boolean newData = false;

void setup() {
  AFMS.begin();  // initialise adafruit library

  // Initialize Pins
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(centerSensor, INPUT);
  pinMode(rightSensor, INPUT);

  Serial.begin(9600);  //Start serial

  // set motor sppeds and directions
  leftMotor->setSpeed(30);  // initialise the speed + direction for left
  leftMotor->run(BACKWARD);

  rightMotor->setSpeed(30);  // initialise the speed + direction for motor right
  rightMotor->run(BACKWARD);

  //Welcome message
  Serial.print("<Arduino is ready>");
  Serial.println("<Valid Commands: <START>, <STOP>");
}

void loop() {
  int startTime = millis();

  //Proccessing received serial input
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
  //Show condition being applied to LF program and prints to serial
  if (newData == true) {
    Serial.print("Command: ");
    Serial.println(receivedChars);
    String command = String(receivedChars);

    if (command == "STOP") {
      start = false;
    } else if (command == "START") {
      start = true;
    } else {
      Serial.println("Invalid Command");
    }
    newData = false;
  }

  // Line Follower Loop
  int leftValue = analogRead(leftSensor);  // sensor values
  int centerValue = analogRead(centerSensor);
  int rightValue = analogRead(rightSensor);
  Serial.println((String)leftValue + "," + centerValue + "," + rightValue + "," + speedLeft + "," + speedRight + "," + startTime);

  // if side sensors detect floor and middle detects tape move forward
  if ((leftValue < threshold) && (centerValue > threshold) && (rightValue < threshold)) {
    // Go forward
    leftMotor->setSpeed(motorSpeed);
    rightMotor->setSpeed(motorSpeed);
    speedLeft = motorSpeed;
    speedRight = motorSpeed;

    lastAction = 0;
  }

  // if drifting to left correct to right
  if ((leftValue > threshold) && (centerValue < threshold) && (rightValue < threshold)) {
    // turn right
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->setSpeed(motorSpeed);
    lastAction = 1;
    speedLeft = motorSpeed * -1;
    speedRight = motorSpeed;
  }

  //if drifting to right correct to left
  if ((leftValue < threshold) && (centerValue < threshold) && (rightValue > threshold)) {
    // turn left
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->setSpeed(motorSpeed);
    lastAction = 2;
    speedLeft = motorSpeed;
    speedRight = motorSpeed * -1;
  }

  //If course is to right go right
  if ((leftValue > threshold) && (centerValue > threshold) && (rightValue < threshold)) {
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->setSpeed(motorSpeed);
    lastAction = 1;
    delay(200);
    speedLeft = motorSpeed * -1;
    speedRight = motorSpeed;
  }

  //If course is to left, go left
  if ((leftValue < threshold) && (centerValue > threshold) && (rightValue > threshold)) {
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->setSpeed(motorSpeed);
    lastAction = 2;
    speedLeft = motorSpeed;
    speedRight = motorSpeed * -1;
    delay(200);
  }

  // if no sensors sense tape, continue doing previous motion
  if ((leftValue < threshold) && (centerValue < threshold) && (rightValue < threshold)) {
    // stop
    if (lastAction == 1) {
      leftMotor->run(BACKWARD);
      rightMotor->run(FORWARD);
      leftMotor->setSpeed(motorSpeed);
      rightMotor->setSpeed(motorSpeed);
      speedLeft = motorSpeed * -1;
      speedRight = motorSpeed;
    }
    if (lastAction == 2) {
      leftMotor->run(FORWARD);
      rightMotor->run(BACKWARD);
      leftMotor->setSpeed(motorSpeed);
      rightMotor->setSpeed(motorSpeed);
      speedLeft = motorSpeed;
      speedRight = motorSpeed * -1;
    } else {
      leftMotor->run(BACKWARD);
      leftMotor->setSpeed(motorSpeed);
      rightMotor->run(BACKWARD);
      rightMotor->setSpeed(motorSpeed);
      speedLeft = motorSpeed;
      speedRight = motorSpeed;
      lastAction = 3;
    }
  }

  //Turn off start condition when stop point is reached
  if ((leftValue > threshold) && (centerValue > threshold) && (rightValue > threshold)) {
    start = false;
  }

  //Stop
  if (start == false) {
    //analogWrite(L_MOTOR, 0);
    leftMotor->run(BACKWARD);
    leftMotor->setSpeed(0);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(0);
    speedLeft = 0;
    speedRight = 0;
    lastAction = 4;
  }
}
