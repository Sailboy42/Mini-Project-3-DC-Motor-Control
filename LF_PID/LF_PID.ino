#define TARGET    //Target IR reading for center sensor
#define Kp 0 //P value in PID
#define Ki  0 //I valuei PID
#define Kd 0 //D value in PID
#define L_MOTOR 6 //Left motor pin
#define R_MOTOR 7  //Right motor pin
#define MAX_SPEED 150 //Max speed to set motors
#define TARGET_SPEED 100  //Target speed to set motors to
#define MIN_SPEED 0 //Minimum speed to set motors to
#define NUM_SENSORS 5 //Number of IR sensors

int IR_data[NUM_SENSORS]; //store IR array data

int lastError  = 0; //stores last error found from PID

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
      IR_data[i] = int(analogRead[i]);
    }

    //PID calculations
    int error = TARGET - position; //determine deviation form target position
    P = error;
    I = I + error;
    D = error - lastError;
    lastError = error;  //Save last error for next iteration
    int adjustSpeed  = P*K_P + I*K_I + D*K_D
  }
  else {
    //Stop motors
    analogWrite(L_MOTOR, 0)
    analogWrite(R_MOTOR, 0)
}
