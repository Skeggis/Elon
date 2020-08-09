/*
TODO: Setup if shot received is damaged then it is discarded.
*/
#include <SoftwareSerial.h>
/* ------------------- Shuttle -------------------------*/

const int shuttleFeedingStepPin = 9;
const int shuttleFeedingDirPin = 10;

/* ------------------- Where to Shoot -------------------------*/

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

int maxUpDownSteps = 50;//Max amount of steps upDown motor can take from the center in either direction.
int maxLeftRightSteps = 50;//Max amount of steps leftDown motor can take from the center in either direction.

int upDownSteps = 0;//The steps upDown motor should take in the current direction
int leftRightSteps = 0;//The steps leftRight motor should take in the current direction

bool test = true;

/* ------------------- BLE -------------------------*/
SoftwareSerial HM10(0, 1); // RX = 0, TX = 1
bool shootersTurnedOn = false; //TODO: change to false when this arduino control when the motors are turned on.
bool shooting = false; //Is true when the motors have started moving into position to shoot.
bool shoot = false; //Is true when a shot has been configured for shooting.

String newShot = "";
String shootingQueue = "";

unsigned long shotDelay = 0;
int leftRight = 0;//From app, 0 is leftmost and maxLeftRight*2 is rightmost.
int upDown = 0;//From app, 0 is upmost and maxUpDown*2 is downmost.
int motorSpeed = 0;

unsigned long previousTime = 0;


void setup() {
  Serial.begin(19200);
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


  //Shuttle Setup
  pinMode(shuttleFeedingStepPin, OUTPUT);
  pinMode(shuttleFeedingDirPin, OUTPUT);
}

void loop() {

  if(test){
    Serial.println("Started loop");
    test = false;
    delay(5000);
    digitalWrite(upDownEnablePin, LOW);
  }

  HM10.listen();
  // if HM10 receives data then read
  while (HM10.available() > 0) { readMessage(); }
  if(shootersTurnedOn && shootingQueue.length() > 0 && !shoot){ configureShot(); } 
  else if( ( shootersTurnedOn && shoot && (millis() - previousTime > shotDelay) ) || shooting){ shootIt(); }
}

bool receivingNewShot = false; //Is true if '{' has been received and not '}'.
void readMessage(){
    char appData = HM10.read(); // save the data in string format, The data received from the app (a single char) 
    //'!' is the signal for play/stop.
    if(appData == '!'){ 
      Serial.println("TURNONSHOOTERS");
      shootersTurnedOn = true; }
    else if(appData == '?'){
      Serial.println("TURNOFFSHOOTERS");
      shootersTurnedOn = false;} 
    else if(!receivingNewShot){
        if(appData == '{'){
          receivingNewShot = true;
          newShot += appData;
          } else { Serial.write(appData); }
     } 
     else {
      //First we wait to receive a whole shot {...}, so we save each char
      //in the new_shot string.
      newShot += appData;

      //When this current shot received from the app has been transmitted fully to
      //elon we add the shot (new_shot) to the back of the queue (end of shooting_queue
      //string and clean upp the new_shot string.
      if(appData == '}'){
        receivingNewShot = false;
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
        if(leftRight < 0){ leftRight = 0;}
        else if (leftRight > maxLeftRightSteps*2) { leftRight = maxLeftRightSteps; }
        //Turn motor left.
        if(leftRight < maxLeftRightSteps){ leftRightSteps = maxLeftRightSteps - leftRight; }
        //Turn motor right / stay in center if leftRightSteps == maxLeftRightSteps
        else { leftRightSteps = leftRight - maxLeftRightSteps; }
        break;
      case 2:
        upDown = value;
        
        if(upDown < 0){upDown = 0;}
        else if(upDown > maxUpDownSteps*2){upDown = maxUpDownSteps;}
        //Turn motor up.
        if(upDown < maxUpDownSteps){ upDownSteps = maxUpDownSteps - upDown; }
        //Turn motor down / stay in center if upDownSteps == maxUpDownSteps
        else { upDownSteps = upDown - maxUpDownSteps; }

        
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
  
  shoot = true;
  }


unsigned long timeWhenShootingBegan = 0;
unsigned long movingDelay = 0;

void setMotorDirections(){
  if(shooting){
    if(upDown < 50){ digitalWrite(upDownDirPin, HIGH); } 
      else {digitalWrite(upDownDirPin, LOW); }

      if(leftRight < 50){digitalWrite(leftRightDirPin, HIGH);  }
      else { digitalWrite(leftRightDirPin, LOW);  }
    } else {
      if(upDown < 50){ digitalWrite(upDownDirPin, LOW); } 
      else {digitalWrite(upDownDirPin, HIGH); }

      if(leftRight < 50){digitalWrite(leftRightDirPin, LOW);  }
      else { digitalWrite(leftRightDirPin, HIGH);  }
      }
  
}
  
  void shootIt(){  
    if(!shooting){
      shooting = true;
      setMotorDirections();
      moveDirectionMotors();
      timeWhenShootingBegan = millis();
      }

if(millis() - timeWhenShootingBegan > movingDelay && shooting){
    shooting = false;
    //shoot
  Serial.println("FIRE Boy!");
  shootShuttle();
  Serial.println("Back to Basics");
  setMotorDirections();
  moveDirectionMotors();

  Serial.println("Shot fired: " + String(shotDelay) + " " + leftRightSteps + " " + upDownSteps + " " + motorSpeed);
      
  resetVariablesAfterShooting();
  } 
}

void resetVariablesAfterShooting(){
  shotDelay = 0;
      leftRightSteps = 0;
      upDownSteps = 0;
      motorSpeed = 0;
      leftRight = 0;
      upDown = 0;
      previousTime = 0;
      shoot = false;
  }
