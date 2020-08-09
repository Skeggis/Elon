bool resetDirectionChanged = false;
bool doneResettingShuttleMotors = false;
void resetEverything(){

  bool doneResettingMotors = shouldConfigureShot;

  if(shouldMoveMotorsIntoPositionForShot){
    if(movingMotors){
      if(!resetDirectionChanged){ 
        resetDirectionChanged = true;
        setDirectionForDirectionMotorsToReturnToNeutral();  //Direction_Motors.ino
        }
      
      doneResettingMotors = moveDirectionMotors(true);//Direction_Motors.ino
      } else { doneResettingMotors = true; }
    } else if (shooting){

      if(movingMotors){
        if(!resetDirectionChanged){
          resetDirectionChanged = true;
          shuttleResetMotorsDirection(); //Shuttle.ino
          }

          if(!doneResettingShuttleMotors){ 
            resetDirectionChanged = false;
            //!isShuttleResetting because if isShuttleResetting = 'true' then the resetting has already started implicitly within the main loop so we don't want to rewind that progress.
            doneResettingShuttleMotors = shootShuttle(!getIsShuttleResetting()); //Shuttle.ino
            }
        
        } else { doneResettingShuttleMotors = true; }


//When shuttle motors are back to neutral we need to bring the direction motors (for the shooters) also back to neutral.
        if(doneResettingShuttleMotors){
            
            if(!resetDirectionChanged){
              resetDirectionChanged = true;
              setDirectionForDirectionMotorsToReturnToNeutral();//Direction_Motors.ino
              } 

              //Here reset='false' because Direction_Motors_i should be 0. 
            doneResettingMotors = moveDirectionMotors(false); //Direction_Motors.ino
        }
      
      } else if(shouldResetAfterShooting){

        if(movingMotors){
          doneResettingMotors = moveDirectionMotors(false);//Direction_Motors.ino
          } else {
            movingMotors = true;
            setDirectionForDirectionMotorsToReturnToNeutral();//Direction_Motors.ino
            }
        }

  if(doneResettingMotors){
    doneResettingShuttleMotors = false;
    shootingQueue = "";
    resetDirectionChanged = false;
    resetVariablesAfterShooting();//Elon.ino
    reset = false;
    }
  
  }
