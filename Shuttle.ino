//const int shuttleFeedingStepPin = 9;
//const int shuttleFeedingDirPin = 10;

//const int getShuttleMotorSpeed = 1000; //microseconds per step
//const int rightGetShuttleStepPin = 11;
//const int rightGetShuttleDirPin = 12;
//const int leftGetShuttleStepPin = 13;
//const int leftGetShuttleDirPin = 14;


//void setup() {
  //pinMode(shuttleFeedingStepPin, OUTPUT);
  //pinMode(shuttleFeedingDirPin, OUTPUT);
  //pinMode(leftGetShuttleStepPin, OUTPUT);
  //pinMode(leftGetShuttleDirPin, OUTPUT);
  //pinMode(rightGetShuttleStepPin, OUTPUT);
  //pinMode(rightGetShuttleDirPin, OUTPUT);
  
  //digitalWrite(leftGetShuttleDirPin, LOW);
  //digitalWrite(rightGetShuttleDirPin, LOW);

//}

//void loop() {
  //getShuttle();
  //delay(1000);
  //shootShuttle();
  //delay(getSpeed());
//}

int getSpeed(){
  int in = analogRead(A0);
  int temp = map(analogRead(A0), 0, 1023, 0, 3000);
  return temp;
}

//void getShuttle(){
  //for(int i=0; i<200*16; i++){
    //digitalWrite(leftGetShuttleStepPin, HIGH);
    //digitalWrite(rightGetShuttleStepPin, HIGH);
    //delayMicroseconds(getShuttleMotorSpeed);
    //digitalWrite(leftGetShuttleStepPin, LOW);
    //digitalWrite(rightGetShuttleStepPin, LOW);
    //delayMicroseconds(getShuttleMotorSpeed);
  //}
//}

void shootShuttle(){
  Serial.println("ShootShuttle");
  digitalWrite(shuttleFeedingDirPin, LOW); 

  for(int x = 0; x < 220; x++) {
    digitalWrite(shuttleFeedingStepPin,HIGH); 
    delayMicroseconds(700); 
    digitalWrite(shuttleFeedingStepPin,LOW); 
    delayMicroseconds(700); 
  }
  
  digitalWrite(shuttleFeedingDirPin, HIGH); 
  
  for(int x = 0; x < 220; x++) {
    digitalWrite(shuttleFeedingStepPin,HIGH);
    delayMicroseconds(700);
    digitalWrite(shuttleFeedingStepPin,LOW);
    delayMicroseconds(700);
  }
}
