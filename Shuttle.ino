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

int getSpeed()
{
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

unsigned long shuttleStepSpeed = 2000; //Microseconds
unsigned long timeOfShuttleRun = 0;

bool highShuttleMotors = true;
int iShuttleMotorStep = 0;

int shuttleSteps = 200*2;

bool isShuttleResetting = false;

bool getIsShuttleResetting() { return isShuttleResetting; }

int shuttleForward = HIGH;
int shuttleBackward = LOW;

bool shootShuttle(bool reset)
{
  if (micros() - timeOfShuttleRun < shuttleStepSpeed)
  {
    return false;
  }

  if (iShuttleMotorStep < shuttleSteps)
  {
    int voltage;
    if (highShuttleMotors)
    {
      voltage = HIGH;
    }
    else
    {
      voltage = LOW;
    }
    digitalWrite(shuttleFeedingStepPin, voltage);

    highShuttleMotors = !highShuttleMotors;

    iShuttleMotorStep++;
    timeOfShuttleRun = micros();
    return false;
  }
  else
  {

    Serial.println("ShuttleSteps: " + String(shuttleSteps) + " i: " + String(iShuttleMotorStep));
    timeOfShuttleRun = 0;
    iShuttleMotorStep = 0;
    isShuttleResetting = false;
    return true;
  }
}

//Returns 'true' iff moving motors is done!
//reset is 'true' iff motors should turn back to neutral
bool shootShuttleTemp(bool reset)
{
  if (micros() - timeOfShuttleRun < shuttleStepSpeed)
  {
    return false;
  }

  if (iShuttleMotorStep == 0)
  {
    digitalWrite(shuttleFeedingDirPin, shuttleForward);
  }
  else if (iShuttleMotorStep == shuttleSteps * 2)
  {
    isShuttleResetting = true;
    digitalWrite(shuttleFeedingDirPin, shuttleBackward);
  }

  int voltage;
  if (highShuttleMotors)
  {
    voltage = HIGH;
  }
  else
  {
    voltage = LOW;
  }
  digitalWrite(shuttleFeedingStepPin, voltage);

  highShuttleMotors = !highShuttleMotors;
  if (!reset)
  {
    iShuttleMotorStep++;
  }
  else
  {
    iShuttleMotorStep--;
  }

  bool doneMoving = iShuttleMotorStep >= shuttleSteps * 4;

  if (reset)
  {
    doneMoving = iShuttleMotorStep <= 0;
  }

  timeOfShuttleRun = micros();
  if (doneMoving)
  {
    Serial.println("ShuttleSteps: " + String(shuttleSteps) + " i: " + String(iShuttleMotorStep));
    timeOfShuttleRun = 0;
    iShuttleMotorStep = 0;
    isShuttleResetting = false;
  }

  return doneMoving;
}

void shuttleResetMotorsDirection()
{
  digitalWrite(shuttleFeedingDirPin, shuttleBackward);
}
