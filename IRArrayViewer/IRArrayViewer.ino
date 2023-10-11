#define NUM_SENSORS 5 //Number of IR sensors

int irData[NUM_SENSORS]; //store IR array data

void setup() {
  Serial.begin(9600); //set baud rate

}

void loop() {
  int irSum = 0;
  //Print in Serial IR readings
  //Cycle through sensors and assigns them to array. Left to right
  for (int i = 0; i < NUM_SENSORS; i++){
    irData[i] = int(analogRead[i]);
    Serial.print((String)"IR "+i+": "+irData[i]);
    irSum += int(irData[i]);
  }

  //Prints target reading
  int v;
  v = (4000*irData[0] + 3000*irData[1] + 2000*irData[2] + 1000*irData[3] + 0*irData[4])/(irData[0] + irData[1] + irData[2] + irData[3] + irData[4]);
  Serial.print((String)"Position = " +v+ ", IR Sum: " +irSum);

  delay(250);
}
