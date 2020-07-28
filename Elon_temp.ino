int directionSpeed = 5000; //Microseconds

void moveDirectionMotors(){
  if(upDownSteps > leftRightSteps){
    for(int i = 0; i<leftRightSteps; i++){
      digitalWrite(upDownStepPin, HIGH);
      digitalWrite(leftRightStepPin, HIGH);
      delayMicroseconds(directionSpeed);
      digitalWrite(upDownStepPin, LOW);
      digitalWrite(leftRightStepPin, LOW);
      delayMicroseconds(directionSpeed);
    }
    
    for(int i = 0; i < upDownSteps - leftRightSteps; i++){
      digitalWrite(upDownStepPin, HIGH);
      delayMicroseconds(directionSpeed);
      digitalWrite(upDownStepPin, LOW);
      delayMicroseconds(directionSpeed);
    }
  } else {
    for(int i = 0; i<upDownSteps; i++){
      digitalWrite(upDownStepPin, HIGH);
      digitalWrite(leftRightStepPin, HIGH);
      delayMicroseconds(directionSpeed);
      digitalWrite(upDownStepPin, LOW);
      digitalWrite(leftRightStepPin, LOW);
      delayMicroseconds(directionSpeed);
    }
    
    for(int i = 0; i< leftRightSteps - upDownSteps; i++){
      digitalWrite(leftRightStepPin, HIGH);
      delayMicroseconds(directionSpeed);
      digitalWrite(leftRightStepPin, LOW);
      delayMicroseconds(directionSpeed);
    }
  }

}
