unsigned long directionSpeed = 12000; //Microseconds
unsigned long timeOfDirectionMotorRun = 0;

bool highDirectionMotors = true;
int iDirectionMotors = 0;

int getIDirectionMotors(){ return iDirectionMotors; } 

//Returns 'true' iff moving motors is done!
//reset is true if the function should count down.
bool moveDirectionMotors(bool reset){
  if(micros() - timeOfDirectionMotorRun < directionSpeed) { return false; }

  int voltage;
  if(highDirectionMotors) { voltage = HIGH; }
  else { voltage = LOW; } 

  if( iDirectionMotors < leftRightSteps*2 ) { digitalWrite(leftRightStepPin, voltage); }

  if( iDirectionMotors < upDownSteps*2 ) { digitalWrite(upDownStepPin, voltage); }
  
  highDirectionMotors = !highDirectionMotors;
  if(!reset){iDirectionMotors++;}
  else { iDirectionMotors--; }
  
  timeOfDirectionMotorRun = micros();

  bool doneMoving = iDirectionMotors >= upDownSteps*2 && iDirectionMotors >= leftRightSteps*2; 
  if(reset) { doneMoving = iDirectionMotors <= 0; }

  if(doneMoving){
    Serial.println("LeftRightSteps: " + String(leftRightSteps) + " UpDownSteps: " + String(upDownSteps) + " i: " + String(iDirectionMotors));
    iDirectionMotors = 0;
    timeOfDirectionMotorRun = 0;
    }
    
  return doneMoving;
   
  }


int up = LOW;
int down = HIGH;
int left = LOW;
int right = HIGH;

void setDirectionForDirectionMotorsBeforeShot(){
      if(upDown < 50){ digitalWrite(upDownDirPin, up); } 
      else {digitalWrite(upDownDirPin, down); }

      if(leftRight < 50){digitalWrite(leftRightDirPin, left);  }
      else { digitalWrite(leftRightDirPin, right);  }
  }

  void setDirectionForDirectionMotorsToReturnToNeutral(){

    if(upDown < 50){ digitalWrite(upDownDirPin, down); } 
    else {digitalWrite(upDownDirPin, up); }

    if(leftRight < 50){digitalWrite(leftRightDirPin, right);  }
    else { digitalWrite(leftRightDirPin, left);  }
    
    }





  
