#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1); // we are using M1


void setup() {
  // put your setup code here, to run once:
AFMS.begin();
  myMotor->setSpeed(50);
  myMotor->run(BACKWARD);

}

void loop() {
  // put your main code here, to run repeatedly:

}



