void setup() {
  Serial.begin(9600); //set boad rate

}

void readline() {
  //Print in Serial IR readings
  a[0] = anologRead(0);
  Serial.print(a[0]);
  a[1] = anologRead(1);
  Serial.print(a[1]);
  a[2] = anologRead(2);
  Serial.print(a[2]);
  a[3] = anologRead(3);
  Serial.print(a[3]);
  a[4] = anologRead(4);
  Serial.print(a[4]);

  //Prints target reading
  /*
 int v;
  v = (4000*a[0] + 3000*a[1] + 2000*a[2] + 1000*a[3] + 0*a[4])/(a[0] + a[1] + a[2] + a[3] + a[4]);
  Serial.print(“Position = “);
  Serial.println(v);
  return v;
}
*/
}
