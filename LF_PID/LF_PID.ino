#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3); // we are using M3
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1); // we are using M1

#define TARGET  2031  //Target IR reading for center sensor
#define Kp 0 //P value in PID
#define Ki  0 //I value in PID
#define Kd 0 //D value in PID
#define lMOTOR 6 //Left motor pin
#define rMOTOR 7  //Right motor pin
#define maxSPEED 150 //Max speed to set motors
#define targetSPEED 100  //Target speed to set motors to
#define minSPEED -150 //Minimum speed to set motors to
#define numSENSORS 5 //Number of IR sensors
#define stopVALUE 4550 //Value of IR sensors at finish line

int irData[numSENSORS]; //store IR array data

int lastError  = 0; //stores last error found from PID

int lmDirection = FORWARD;
int rmDirection = FORWARD;
int lmSpeed = 0;
int rmSpeed = 0;
int irAverage = 0;
int irSum = 0;

bool start = true;

//Serial control setup
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;

void setup() {
  AFMS.begin();
  // Initialize Pins
  pinMode(lMOTOR, OUTPUT);
  pinMode(rMOTOR, OUTPUT);
  Serial.begin(9600); //Start  serial

  leftMotor->setSpeed(lmSpeed); // initialise the speed + direction for left
  leftMotor->run(lmDirection);

  rightMotor->setSpeed(rmSpeed);  // initialise the speed + direction for motor one
  rightMotor->run(rmDirection);

  Serial.print("<Arduino is ready>");
  Serial.println("<Valid Commands: <START>, <STOP>");
}

void loop() {
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
          }
          else {
              receivedChars[ndx] = '\0'; // terminate the string
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
        }
        else if (command == "START") {
          start = true;
        }
        else {
          Serial.println("Invalid Command");
          
        }
        newData = false;
  }
  //Line follower loop begins
  if (start == true) { //will stop program if end condition is met
    //Serial.print("Start if");
    int irAverage = 0;
    int irSum = 0;

    //Cycle through sensors and assigns them to array. Left to right
    for (int i = 0; i < numSENSORS; i++){
      irData[i] = analogRead(i);
      irAverage += irData[i] * i * 1000;   //weighted mean   
      irSum += irData[i];
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
      //lmDirection = FORWARD;
      lmSpeed = HIGH;
    }
    else {
      //lmDirection = BACKWARD;
      lmSpeed = LOW;
    }
    if (rmSpeed >= 0) {
      //rmDirection = FORWARD;
      rmSpeed = HIGH;
    }
    else {
      //rmDirection = BACKWARD;
      rmSpeed = LOW;
    }

    //Assure motor speed is not leaving limits of motor function
    /*
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
    */
    //analogWrite(L_MOTOR, lmSpeed);
   // leftMotor->setSpeed(abs(lmSpeed));
    //leftMotor->run(lmDirection);
    digitalWrite(lMOTOR, 255);

    //analogWrite(R_MOTOR, rmSpeed);
   // rightMotor->setSpeed(abs(rmSpeed));
   // rightMotor->run(rmDirection);
    
    //Data to serial for graphing in python
    for (int i = 0; i < numSENSORS; i++){
      //Serial.println(analogRead(i));
      Serial.print(irData[i]);
      Serial.print(",");
    }
    Serial.print(lmSpeed);
    Serial.print(",");
    Serial.println(rmSpeed);
    
  }

  else {
    //Serial.print("Else start");
    //Stop motors
    //analogWrite(L_MOTOR, 0);
    leftMotor->run(RELEASE);
    //analogWrite(R_MOTOR, 0);
    rightMotor->run(RELEASE);
    /*
    for (int i = 0; i < numSENSORS; i++){
      //Serial.println(analogRead(i));
      Serial.print(irData[i]);
      Serial.print(",");
    }
    Serial.print(lmSpeed);
    Serial.print(",");
    Serial.println(rmSpeed);
    */
  }
}
