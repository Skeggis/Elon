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


unsigned long shuttleStepSpeed = 700; //Microseconds
unsigned long timeOfShuttleRun = 0;

bool highShuttleMotors = true;
int iShuttleMotorStep = 0;

int shuttleSteps = 220;

//Returns 'true' iff moving motors is done!
bool shootShuttle(){
  if(millis() - timeOfShuttleRun < shuttleStepSpeed) { return false; }

  if(iShuttleMotorStep == 0) { digitalWrite(shuttleFeedingDirPin, LOW);  }
  else if(iShuttleMotorStep == shuttleSteps) {digitalWrite(shuttleFeedingDirPin, HIGH); }

  int voltage;
  if(highShuttleMotors) { voltage = HIGH; }
  else { voltage = LOW; } 
  digitalWrite(shuttleFeedingStepPin,voltage);

  highShuttleMotors = !highShuttleMotors;
  if(highShuttleMotors) { iShuttleMotorStep++; }

  bool doneMoving = iShuttleMotorStep >= shuttleSteps*2;

  timeOfShuttleRun = millis();
  if(doneMoving){ 
    timeOfShuttleRun = 0;
    iShuttleMotorStep = 0;
    }
    
  return doneMoving;
}
