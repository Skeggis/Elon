/*

*/
#include <SoftwareSerial.h>

int upDownStepPin = 5;
int upDownDirPin = 6; //LOW => down, HIGH => up
int upDownEnablePin = 2;

int leftRightStepPin = 7;
int leftRightDirPin = 8; //HIGH => left, LOW => right

// 0 => resting position
// 1 => topLeft
// 2 => topRight
// 3 => bottomRight
// 4 => bottomLeft
int position = 0;

int upDownSteps = 50;
int leftRightSteps = 50;

int directionSpeed = 5000; //Microseconds

bool test = true;

/* ------------------- BLE -------------------------*/
SoftwareSerial HM10(3, 4); // RX = 3, TX = 4
bool shootersTurnedOn = false; //TODO: change to false when this arduino control when the motors are turned on.
bool shooting = false;

String newShot = "";
String shootingQueue = "";

unsigned long shotDelay = 0;
int leftRight = 0;
int upDown = 0;
int motorSpeed = 0;

unsigned long previousTime = 0;


void setup() {
  Serial.begin(9600);
  pinMode(upDownStepPin, OUTPUT); 
  pinMode(upDownDirPin, OUTPUT);
  pinMode(leftRightStepPin, OUTPUT);
  pinMode(leftRightDirPin, OUTPUT);
  pinMode(upDownEnablePin, OUTPUT);
  digitalWrite(upDownEnablePin, HIGH);
  //digitalWrite(leftRightStepPin, HIGH);
  //digitalWrite(upDownStepPin, HIGH);
Serial.println("HM10 serial started at 9600");
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
  
}

void loop() {

  //if(test){
    //Serial.println("Started loop");
    //test = false;
    //delay(5000);
    //digitalWrite(upDownEnablePin, LOW);
  //}

  HM10.listen();
  // if HM10 receives data then read
  while (HM10.available() > 0) { readMessage(); }

  if(shootersTurnedOn && shootingQueue.length() > 0 && !shooting){ configureShot(); } 
  else if(shootersTurnedOn && shooting && (millis() - previousTime > shotDelay)){ shoot(); }
}

void readMessage(){
    char appData = HM10.read(); // save the data in string format, The data received from the app (a single char) 
    //'!' is the signal for play/stop.
    if(appData == '!'){ shootersTurnedOn = !shootersTurnedOn; } 
    else {
      //First we wait to receive a whole shot {...}, so we save each char
      //in the new_shot string.
      newShot += appData;

      //When this current shot received from the app has been transmitted fully to
      //elon we add the shot (new_shot) to the back of the queue (end of shooting_queue
      //string and clean upp the new_shot string.
      if(appData == '}'){
        shootingQueue += newShot;
        newShot = "";
        Serial.println("Queue growing larger: " + shootingQueue);
       }
    }
}

void configureShot(){

  //Extract next shot:
  String delimiter = "}";
  int pos = shootingQueue.indexOf(delimiter);
  String shot = shootingQueue.substring(0, pos+1);
  Serial.println("Shot to configure " + shot);
  
  //Delete the shot just extracted from the queue.
  shootingQueue.remove(0, pos+delimiter.length());

  pos = shot.indexOf("{");
  //Delete '{' and '}' from the shot string.
  shot.remove(0,pos + 1);
  shot.remove(shot.length()-1, 1);
  
  //Extract the values for the shot: shot_delay, left_right, up_down, motor_speed
    for(int i = 0; i < 4; i++){
    pos = shot.indexOf(',');
    String subString = shot.substring(0, pos);
    long value = (subString).toInt();
    shot.remove(0, pos + 1);
    switch(i){
      case 0:
        shotDelay = value;
        break;
      case 1:
        leftRight = value;
        break;
      case 2:
        upDown = value;
        break;
      case 3:
        motorSpeed = value;
        break;
      default:
        break;
      }
    }
      Serial.println("Delay for next shot: " + String(shotDelay));


  previousTime = millis();
  
  shooting = true;
  }

  void shoot(){
    Serial.println("Fire! " + String(shotDelay) + " " + leftRight + " " + upDown + " " + motorSpeed);

  
    if(upDown < 50){
        if(leftRight < 50){
            shootBottomLeft();
        } else {
            shootBottomRight();
        }
    } else if(upDown > 50){
        if(leftRight < 50){
            shootTopLeft();
        } else {
            shootTopRight();
        }
    }
    shooting = false;
  shotDelay = 0;
  leftRight = 0;
  upDown = 0;
  motorSpeed = 0;
  previousTime = 0;
}

// void shootTopLeft(){ 
//   digitalWrite(upDownDirPin, HIGH); 
//   digitalWrite(leftRightDirPin, HIGH); 
  
//   moveDirectionMotors();
  
//   delay(3000);
//   //shoot
  
//   digitalWrite(upDownDirPin, LOW); 
//   digitalWrite(leftRightDirPin, LOW);
  
//   moveDirectionMotors();
// }


// void shootTopRight(){
//   digitalWrite(upDownDirPin, HIGH);
//   digitalWrite(leftRightDirPin, LOW);
  
//   moveDirectionMotors();
  
//   delay(3000);
  
//   digitalWrite(upDownDirPin, LOW);
//   digitalWrite(leftRightDirPin, HIGH);
  
//   moveDirectionMotors();
// }

// void shootBottomRight(){
//   digitalWrite(upDownDirPin, LOW);
//   digitalWrite(leftRightDirPin, LOW);
  
//   moveDirectionMotors();
  
//   delay(3000);
  
//   digitalWrite(upDownDirPin, HIGH);
//   digitalWrite(leftRightDirPin, HIGH);
  
//   moveDirectionMotors();
// }

// void shootBottomLeft(){
//   digitalWrite(upDownDirPin, LOW);
//   digitalWrite(leftRightDirPin, HIGH);
  
//   moveDirectionMotors();
  
//   delay(3000);
  
//   digitalWrite(upDownDirPin, HIGH);
//   digitalWrite(leftRightDirPin, LOW);
  
//   moveDirectionMotors();
// }

// void moveDirectionMotors(){
//   if(upDownSteps > leftRightSteps){
//     for(int i = 0; i<leftRightSteps; i++){
//       digitalWrite(upDownStepPin, HIGH);
//       digitalWrite(leftRightStepPin, HIGH);
//       delayMicroseconds(directionSpeed);
//       digitalWrite(upDownStepPin, LOW);
//       digitalWrite(leftRightStepPin, LOW);
//       delayMicroseconds(directionSpeed);
//     }
    
//     for(int i = 0; i < upDownSteps - leftRightSteps; i++){
//       digitalWrite(upDownStepPin, HIGH);
//       delayMicroseconds(directionSpeed);
//       digitalWrite(upDownStepPin, LOW);
//       delayMicroseconds(directionSpeed);
//     }
//   } else {
//     for(int i = 0; i<upDownSteps; i++){
//       digitalWrite(upDownStepPin, HIGH);
//       digitalWrite(leftRightStepPin, HIGH);
//       delayMicroseconds(directionSpeed);
//       digitalWrite(upDownStepPin, LOW);
//       digitalWrite(leftRightStepPin, LOW);
//       delayMicroseconds(directionSpeed);
//     }
    
//     for(int i = 0; i< leftRightSteps - upDownSteps; i++){
//       digitalWrite(leftRightStepPin, HIGH);
//       delayMicroseconds(directionSpeed);
//       digitalWrite(leftRightStepPin, LOW);
//       delayMicroseconds(directionSpeed);
//     }
//   }

// }
