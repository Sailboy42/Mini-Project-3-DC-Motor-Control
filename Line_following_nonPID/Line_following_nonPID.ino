#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Setting up Motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3); // we are using M3 port
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2); // we are using M1 port


#define leftSensor  A2 
#define centerSensor A1
#define rightSensor  A0     // sensors defined
#define L_MOTOR 6 //Left motor pin
#define R_MOTOR 7  //Right motor pin
#define threshold 850
#define motor_speed 30
int lastAction = 0;

void setup() {
  AFMS.begin(); // initialise adafruit library
  // Initialize Pins
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(centerSensor, INPUT);
  pinMode(rightSensor, INPUT);
  Serial.begin(9600); //Start serial
  
  // set motor sppeds and directions
  leftMotor->setSpeed(30); // initialise the speed + direction for left
  leftMotor->run(BACKWARD);

  rightMotor->setSpeed(30);  // initialise the speed + direction for motor right
  rightMotor->run(BACKWARD);
}



void loop() {
  // put your main code here, to run repeatedly:
int leftValue = analogRead(leftSensor); // sensor values
int centerValue = analogRead(centerSensor);
int rightValue = analogRead(rightSensor);
Serial.println((String)" left: "+leftValue+" center: "+centerValue+" right: "+rightValue);



// if side sensors detect floor and middle detects tape move forward
if ((leftValue < threshold) && (centerValue > threshold) && (rightValue < threshold)) {
  // Go forward
  leftMotor->setSpeed(30);
  rightMotor->setSpeed(30);
  lastAction = 0;
}
// If left sensor senses tape and center & left don't
if ((leftValue > threshold) && (centerValue < threshold) && (rightValue < threshold)){
  // turn right
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(25);
  rightMotor->setSpeed(25);
  lastAction = 1;

}
// If right sensor senses tape and the others don't
if ((leftValue < threshold) && (centerValue < threshold) && (rightValue > threshold)){
  // turn left
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(25);
  rightMotor->setSpeed(25);
  lastAction = 2;
}

// if middle and left sensor sense tape
if ((leftValue > threshold) && (centerValue > threshold) && (rightValue < threshold))
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(25);
  rightMotor->setSpeed(25);
  lastAction = 1;
}

if ((leftValue < threshold) && (centerValue > threshold) && (rightValue > threshold))
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(25);
  rightMotor->setSpeed(25);
  lastAction = 2;
}
// if no sensors sense tape
if ((leftValue < threshold) && (centerValue < threshold) && (rightValue < threshold)) {
  // stop
      //analogWrite(L_MOTOR, 0);
  leftMotor->run(BACKWARD);
  leftMotor->setSpeed(25);
  //analogWrite(R_MOTOR, 0);
  rightMotor->run(BACKWARD);
  rightMotor->setSpeed(25);
  lastAction = 3;

}

// Turning situation

//if ((leftValue < threshold)&& (centerValue< threshold)&& (rightValue > threshold)){
  //power right
 // leftMotor->run(FORWARD);
 // rightMotor->run(BACKWARD);
//  leftMotor->setSpeed(25);
 // rightMotor->setSpeed(25);
//}
//if ((leftValue > threshold)&& (centerValue < threshold)&& (rightValue < threshold)){
  //power left
//  leftMotor->run(BACKWARD);
//  rightMotor->run(FORWARD);
//  leftMotor->setSpeed(25);
//  rightMotor->setSpeed(25);
//}


}

