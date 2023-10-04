#define TARGET    //Target IR reading for center sensor
#define Kp 0 //P value in PID
#define Ki  0 //I valuei PID
#define Kd 0 //D value in PID
#define L_MOTOR 6 //Left motor pin
#define R_MOTOR 7  //Right motor pin
#define maxSPEED 150 //Max speed to set motors
#define targetSPEED 100  //Target speed to set motors to
#define minSPEED -150 //Minimum speed to set motors to
#define NUM_SENSORS 5 //Number of IR sensors

int irData[NUM_SENSORS]; //store IR array data

int lastError  = 0; //stores last error found from PID

int lmSpeed = 0;
int rmSpeed = 0;
int irAverage = 0;
int irSum = 0;

bool start = true;

void setup() {
  // Initialize Pins
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  Serial.begin(9600); //Start  serial
}

void loop() {
  if (start == true) { //will stop program if end condition is met

    //Cycle through sensors and assigns them to array. Left to right
    for (int i = 0; i <= NUM_SENSORS; i++){
      irData[i] = int(analogRead[i]);
      int irAverage = irData[i] * i * 1000;   //weighted mean   
      int irSum = int(irData[i]);
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
  
  analogWrite(L_MOTOR, lmSpeed);
  analogWrite(R_MOTOR, rmSpeed);
  }

  else {
    //Stop motors
    analogWrite(L_MOTOR, 0);
    analogWrite(R_MOTOR, 0);
  }
}
