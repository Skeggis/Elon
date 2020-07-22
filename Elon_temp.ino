// /*

// */

// int upDownStepPin = 5;
// int upDownDirPin = 6; //LOW => down, HIGH => up
// int upDownEnablePin = 2;

// int leftRightStepPin = 7;
// int leftRightDirPin = 8; //HIGH => left, LOW => right

// // 0 => resting position
// // 1 => topLeft
// // 2 => topRight
// // 3 => bottomRight
// // 4 => bottomLeft
// int position = 0;

// int upDownSteps = 50;
// int leftRightSteps = 50;

// int directionSpeed = 5000; //Microseconds

// bool test = true;

// void setup() {
//   Serial.begin(9600);
//   pinMode(upDownStepPin, OUTPUT); 
//   pinMode(upDownDirPin, OUTPUT);
//   pinMode(leftRightStepPin, OUTPUT);
//   pinMode(leftRightDirPin, OUTPUT);
//   pinMode(upDownEnablePin, OUTPUT);
//   digitalWrite(upDownEnablePin, HIGH);
//   //digitalWrite(leftRightStepPin, HIGH);
//   //digitalWrite(upDownStepPin, HIGH);
  
// }

// void loop() {

//   if(test){
//     Serial.println("Started loop");
//     test = false;
//     delay(5000);
//     digitalWrite(upDownEnablePin, LOW);
//   }
  
//   shootTopLeft();
//   delay(1000);
//   shootTopRight();
//   delay(1000);
//   shootBottomRight();
//   delay(1000);
//   shootBottomLeft();
//   delay(1000);
  
  
// }

void shootTopLeft(){ 
  digitalWrite(upDownDirPin, HIGH); 
  digitalWrite(leftRightDirPin, HIGH); 
  
  moveDirectionMotors();
  
  delay(3000);
  //shoot
  
  digitalWrite(upDownDirPin, LOW); 
  digitalWrite(leftRightDirPin, LOW);
  
  moveDirectionMotors();
}


void shootTopRight(){
  digitalWrite(upDownDirPin, HIGH);
  digitalWrite(leftRightDirPin, LOW);
  
  moveDirectionMotors();
  
  delay(3000);
  
  digitalWrite(upDownDirPin, LOW);
  digitalWrite(leftRightDirPin, HIGH);
  
  moveDirectionMotors();
}

void shootBottomRight(){
  digitalWrite(upDownDirPin, LOW);
  digitalWrite(leftRightDirPin, LOW);
  
  moveDirectionMotors();
  
  delay(3000);
  
  digitalWrite(upDownDirPin, HIGH);
  digitalWrite(leftRightDirPin, HIGH);
  
  moveDirectionMotors();
}

void shootBottomLeft(){
  digitalWrite(upDownDirPin, LOW);
  digitalWrite(leftRightDirPin, HIGH);
  
  moveDirectionMotors();
  
  delay(3000);
  
  digitalWrite(upDownDirPin, HIGH);
  digitalWrite(leftRightDirPin, LOW);
  
  moveDirectionMotors();
}

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
