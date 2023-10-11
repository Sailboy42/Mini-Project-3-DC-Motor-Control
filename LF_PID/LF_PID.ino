#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3); // we are using M3
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1); // we are using M1

#define TARGET    //Target IR reading for center sensor
#define Kp 0 //P value in PID
#define Ki  0 //I value in PID
#define Kd 0 //D value in PID
#define L_MOTOR 6 //Left motor pin
#define R_MOTOR 7  //Right motor pin
#define maxSPEED 150 //Max speed to set motors
#define targetSPEED 100  //Target speed to set motors to
#define minSPEED -150 //Minimum speed to set motors to
#define NUM_SENSORS 5 //Number of IR sensors
#define stopVALUE 5000 //Value of IR sensors at finish line

int irData[NUM_SENSORS]; //store IR array data

int lastError  = 0; //stores last error found from PID

int lmDirection = FORWARD;
int rmDirection = FORWARD;
int lmSpeed = 0;
int rmSpeed = 0;
int irAverage = 0;
int irSum = 0;

bool start = true;

void setup() {
  AFMS.begin();
  // Initialize Pins
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  Serial.begin(9600); //Start  serial

  leftMotor->setSpeed(lmSpeed); // initialise the speed + direction for left
  leftMotor->run(lmDirection);

  rightMotor->setSpeed(rmSpeed);  // initialise the speed + direction for motor one
  rightMotor->run(rmDirection);

}

void loop() {
  if (start == true) { //will stop program if end condition is met

    int irAverage = 0;
    int irSum = 0;

    //Cycle through sensors and assigns them to array. Left to right
    for (int i = 0; i < NUM_SENSORS; i++){
      irData[i] = int(analogRead(i));
      irAverage += irData[i] * i * 1000;   //weighted mean   
      irSum += int(irData[i]);
    }
    if (irSum >= stopVALUE) { //checks to see if sensors are over finish line
      start = false; //swips loop condition switch
      return; //exits loop iteration
    }

    int position = int(irAverage / irSum);

    //PID calculations
    int error = TARGET - position; //determine deviation form target position
    int p = error;
    int i = Ki + error;
    int d = error - lastError;
    lastError = error;  //Save last error for next iteration
    int adjustSpeed  = p*Kp + i*Ki + d*Kd;

    lmSpeed = targetSPEED + adjustSpeed;
    rmSpeed = targetSPEED - adjustSpeed;

    //Determine motor direction depending on speed
    if (lmSpeed >= 0) {
      lmDirection = FORWARD;
    }
    else {
      lmDirection = BACKWARD;
    }
    if (rmSpeed >= 0) {
      rmDirection = FORWARD;
    }
    else {
      rmDirection = BACKWARD;
    }

    //Assure motor speed is not leaving limits of motor function
    if (lmSpeed > maxSPEED) {
      lmSpeed = maxSPEED;
    }

    if (lmSpeed < minSPEED) {
      lmSpeed = minSPEED;
    }

    if (rmSpeed > maxSPEED) {
      rmSpeed = maxSPEED;
    }

    if (rmSpeed < minSPEED) {
      rmSpeed = minSPEED;
    } 
  
  //analogWrite(L_MOTOR, lmSpeed);
  leftMotor->setSpeed(lmSpeed);
  leftMotor->run(lmDirection);

  //analogWrite(R_MOTOR, rmSpeed);
  rightMotor->setSpeed(rmSpeed);
  rightMotor->run(rmDirection);
  }

  else {
    //Stop motors
  //analogWrite(L_MOTOR, 0);
    leftMotor->run(RELEASE);
    //analogWrite(R_MOTOR, 0);
    rightMotor->run(RELEASE);

  }
}
