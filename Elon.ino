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

int maxUpDownSteps = 50;    //Max amount of steps upDown motor can take from the center in either direction.
int maxLeftRightSteps = 50; //Max amount of steps leftDown motor can take from the center in either direction.

int upDownSteps = 0;    //The steps upDown motor should take in the current direction
int leftRightSteps = 0; //The steps leftRight motor should take in the current direction

bool test = true;

int bleRx = 11;
int bleTx = 12;

/* ------------------- BLE -------------------------*/
SoftwareSerial HM10(bleRx, bleTx);
bool reset = false;                               //If true then take state as is and return everything to neutral.
bool shootersTurnedOn = true;                     //TODO: change to false when this arduino control when the motors are turned on.
bool shouldConfigureShot = true;                  //Is true when a shot needs to be configured
bool shouldMoveMotorsIntoPositionForShot = false; //Is true when a shot is about to be fired, and we need the upDown/leftRight motors to move shooters to their coorect positions
bool shooting = false;                            //Is true when the motors have started moving into position to shoot and the shuttle should be brought to the shooters
bool shouldResetAfterShooting = false;            //Is true when a shot has been fired and we need the motors to return to neutral position.

bool movingMotors = false; //Is true if one of the shooting steps is turning motors, so the code is not blocking, i.e. direction or shuttle.

String newShot = "";
String shootingQueue = "";

unsigned long shotDelay = 0;
int leftRight = 0; //From app, 0 is leftmost and maxLeftRight*2 is rightmost.
int upDown = 0;    //From app, 0 is upmost and maxUpDown*2 is downmost.
int motorSpeed = 0;

unsigned long previousTime = 0;

void setup()
{
    Serial.begin(9600);

    //Shooters Setup
    pinMode(upDownStepPin, OUTPUT);
    pinMode(upDownDirPin, OUTPUT);
    pinMode(leftRightStepPin, OUTPUT);
    pinMode(leftRightDirPin, OUTPUT);
    pinMode(upDownEnablePin, OUTPUT);
    digitalWrite(upDownEnablePin, HIGH);

    Serial.println("HM10 serial started at 9600");
    HM10.begin(9600); // set HM10 serial at 9600 baud rate

    //Shuttle Setup
    pinMode(shuttleFeedingStepPin, OUTPUT);
    pinMode(shuttleFeedingDirPin, OUTPUT);
}

unsigned long timeWhenShootingBegan = 0;
unsigned long movingDelay = 0;

void loop()
{

    if (test)
    {
        Serial.println("Started loop");
        test = false;
        delay(5000);
        digitalWrite(upDownEnablePin, LOW);
    }

    HM10.listen();
    //if HM10 receives data then read
    while (HM10.available() > 0)
    {
        readMessage();
    }

    if (shootersTurnedOn)
    {
        shootingActivity();
    }

    if (reset)
    {
        resetEverything();
    } //Reset.ino
    else
    {
        if (shootersTurnedOn)
        {
            shootingActivity();
        }
    }
}

bool receivingNewShot = false; //Is true if '{' has been received and not '}'.
void readMessage()
{
    char appData = HM10.read(); // save the data in string format, The data received from the app (a single char)
    //'!' is the signal for play/stop.
    if (appData == '!')
    {
        Serial.println("TURNONSHOOTERS");
        shootersTurnedOn = true;
        // HM10.write('$')

    }
    else if (appData == '?')
    {
        Serial.println("TURNOFFSHOOTERS");
        shootersTurnedOn = false;
        // HM10.write('%')

    }
    else if (!receivingNewShot)
    {
        if (appData == '{')
        {
            receivingNewShot = true;
            newShot += appData;
        }
        else
        {
            Serial.write(appData);
        }
    }
    else
    {
        //First we wait to receive a whole shot {...}, so we save each char
        //in the new_shot string.
        newShot += appData;

        //When this current shot received from the app has been transmitted fully to
        //elon we add the shot (new_shot) to the back of the queue (end of shooting_queue
        //string and clean upp the new_shot string.
        if (appData == '}')
        {
            receivingNewShot = false;
            shootingQueue += newShot;
            newShot = "";
            Serial.println("Queue growing larger: " + shootingQueue);
        }
    }
}

void shootingActivity()
{

    if (shouldConfigureShot && shootingQueue.length() > 0)
    {
        //Serial.println("ShouldConfigureShot");
        configureShot();
        previousTime = millis();

        shouldConfigureShot = false;

        /*------NORMAL-----*/
        shouldMoveMotorsIntoPositionForShot = true;

        /*------Skip upDown/LeftRight-----*/
        //shooting = true;
    }
    else if ((shouldMoveMotorsIntoPositionForShot && (millis() - previousTime > shotDelay)))
    {

        if (!movingMotors)
        {
            Serial.println("shouldMoveMotorsIntoPositionForShot");
            movingMotors = true;
            setDirectionForDirectionMotorsBeforeShot(); //Direction_Motors.ino
        }

        bool doneMoving = moveDirectionMotors(false); //Direction_Motors.ino

        if (doneMoving)
        {
            Serial.println("doneMovingIntoPosition");
            movingMotors = false;
            shooting = true;
            shouldMoveMotorsIntoPositionForShot = false;
            timeWhenShootingBegan = millis();
        }
    }
    else if ((shooting && millis() - timeWhenShootingBegan > movingDelay))
    {

        if (!movingMotors)
        {
            Serial.println("Shooting");
            movingMotors = true;
        }
        bool doneMoving = shootShuttle(false); //Shuttle.ino

        if (doneMoving)
        {
            Serial.println("DoneShooting");
            movingMotors = false;
            shooting = false;

            /*------NORMAL-----*/
            shouldResetAfterShooting = true;

            /*------Skip upDown/LeftRight-----*/
            // resetVariablesAfterShooting();
        }
    }
    else if (shouldResetAfterShooting)
    {

        if (!movingMotors)
        {
            Serial.println("Reset");
            movingMotors = true;
            setDirectionForDirectionMotorsToReturnToNeutral(); //Direction_Motors.ino
        }

        bool doneMoving = moveDirectionMotors(false); //Direction_Motors.ino

        if (doneMoving)
        {
            Serial.println("DoneReset");
            movingMotors = false;
            resetVariablesAfterShooting();
            //Signal app that shooting is done
            HM10.write('#');
        }
    }
}

void configureShot()
{
    //Extract next shot:
    String delimiter = "}";
    int pos = shootingQueue.indexOf(delimiter);
    String shot = shootingQueue.substring(0, pos + 1);
    Serial.println("Shot to configure " + shot);

    //Delete the shot just extracted from the queue.
    shootingQueue.remove(0, pos + delimiter.length());

    pos = shot.indexOf("{");
    //Delete '{' and '}' from the shot string.
    shot.remove(0, pos + 1);
    shot.remove(shot.length() - 1, 1);

    //Extract the values for the shot: shot_delay, left_right, up_down, motor_speed
    for (int i = 0; i < 4; i++)
    {
        pos = shot.indexOf(',');
        String subString = shot.substring(0, pos);
        long value = (subString).toInt();
        shot.remove(0, pos + 1);
        switch (i)
        {
        case 0:
            shotDelay = value;
            break;
        case 1:
            leftRight = value;
            if (leftRight < 0)
            {
                leftRight = 0;
            }
            else if (leftRight > maxLeftRightSteps * 2)
            {
                leftRight = maxLeftRightSteps;
            }
            //Turn motor left.
            if (leftRight < maxLeftRightSteps)
            {
                leftRightSteps = maxLeftRightSteps - leftRight;
            }
            //Turn motor right / stay in center if leftRightSteps == maxLeftRightSteps
            else
            {
                leftRightSteps = leftRight - maxLeftRightSteps;
            }
            break;
        case 2:
            upDown = value;

            if (upDown < 0)
            {
                upDown = 0;
            }
            else if (upDown > maxUpDownSteps * 2)
            {
                upDown = maxUpDownSteps;
            }
            //Turn motor up.
            if (upDown < maxUpDownSteps)
            {
                upDownSteps = maxUpDownSteps - upDown;
            }
            //Turn motor down / stay in center if upDownSteps == maxUpDownSteps
            else
            {
                upDownSteps = upDown - maxUpDownSteps;
            }

            break;
        case 3:
            motorSpeed = value;
            break;
        default:
            break;
        }
    }
    //Serial.println("Delay for next shot: " + String(shotDelay));
}

void resetVariablesAfterShooting()
{
    shotDelay = 0;
    leftRightSteps = 0;
    upDownSteps = 0;
    motorSpeed = 0;
    leftRight = 0;
    upDown = 0;
    previousTime = 0;

    movingMotors = false;
    shouldConfigureShot = true;                  //Is true when a shot needs to be configured
    shooting = false;                            //Is true when the motors have started moving into position to shoot.
    shouldMoveMotorsIntoPositionForShot = false; //Is true when a shot is about to be fired, and we need the upDown/leftRight motors to move shooters to their coorect positions
    shouldResetAfterShooting = false;
}
