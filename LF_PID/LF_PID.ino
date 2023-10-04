#define TARGET    //Target IR reading for center sensor
#define K_P    //P value in PID
#define K_D    //D value in PID
#define L_MOTOR 6 //Left motor pin
#define R_MOTOR 7  //Right motor pin
#define MAX_SPEED 150 //Max speed to set motors
#define TARGET_SPEED 100  //Target speed to set motors to
#define MIN_SPEED 0 //Minimum speed to set motors to
#define NUM_SENSORS 5 //Number of IR sensors

int IR_data[NUM_SENSORS]; //store IR array data

int LastError  = 0; //stores last error found from PID

void setup() {
  // Initialize Pins
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  Serial.begin(9600); //Start  serial
}

void loop() {
  for (int i = 0; i <= NUM_SENSORS; i++){
  IR_data[i] = int(analogRead[i]);
  }
}
