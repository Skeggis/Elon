unsigned long directionSpeed = 5000; //Microseconds
unsigned long timeOfDirectionMotorRun = 0;

bool highDirectionMotors = true;
int iDirectionMotors = 0;

//Returns 'true' iff moving motors is done!
bool moveDirectionMotors(){

  if(millis() - timeOfDirectionMotorRun < directionSpeed) { return false; }


  int voltage;
  if(highDirectionMotors) { voltage = HIGH; }
  else { voltage = LOW; } 

  if( iDirectionMotors < leftRightSteps ) { digitalWrite(leftRightStepPin, voltage); }

  if( iDirectionMotors < upDownSteps ) { digitalWrite(upDownStepPin, voltage); }
  
  highDirectionMotors = !highDirectionMotors;
  if(highDirectionMotors){iDirectionMotors++;}
  
  timeOfDirectionMotorRun = millis();

  bool doneMoving = iDirectionMotors >= upDownSteps && iDirectionMotors >= leftRightSteps; 
  if(doneMoving){
    iDirectionMotors = 0;
    timeOfDirectionMotorRun = 0;
    }
    
  return doneMoving;
   
  }


void setDirectionForDirectionMotorsBeforeShot(){
      if(upDown < 50){ digitalWrite(upDownDirPin, HIGH); } 
      else {digitalWrite(upDownDirPin, LOW); }

      if(leftRight < 50){digitalWrite(leftRightDirPin, HIGH);  }
      else { digitalWrite(leftRightDirPin, LOW);  }
  }

  void setDirectionForDirectionMotorsToReturnToNeutral(){

    if(upDown < 50){ digitalWrite(upDownDirPin, LOW); } 
    else {digitalWrite(upDownDirPin, HIGH); }

    if(leftRight < 50){digitalWrite(leftRightDirPin, LOW);  }
    else { digitalWrite(leftRightDirPin, HIGH);  }
    
    }





  
