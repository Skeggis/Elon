// #include <SoftwareSerial.h>
// SoftwareSerial HM10(2, 3); // TX = 2, RX = 3

// bool turned_on = false;
// bool shooting = false;

// String shooting_queue = "";
// String new_shot = "";

// unsigned long shot_delay = 0;
// unsigned long previous_time = 0;
// int left_right = 0;
// int up_down = 0;
// int motor_speed = 0;

// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("HM10 serial started at 9600");
//   HM10.begin(9600); // set HM10 serial at 9600 baud rate
// }

// void loop()
// {
//   HM10.listen();
//   // if HM10 sends something then read
//   while (HM10.available() > 0) { readAppMessage(); }

//   if(turned_on && shooting_queue.length() > 0 && !shooting){ configure_shot(); } 
//   else if(turned_on && shooting && (millis() - previous_time > shot_delay)){ shoot(); }
// }

// void readAppMessage(){
//     char appData = HM10.read(); // save the data in string format, The data received from the app (a single char) 
    
//     //'!' is the signal for play/stop.
//     if(appData == '!'){ turned_on = !turned_on; } 
//     else {
//       //First we wait to receive a whole shot {...}, so we save each char
//       //in the new_shot string.
//       new_shot += appData;

//       //When this current shot received from the app has been transmitted fully to
//       //elon we add the shot (new_shot) to the back of the queue (end of shooting_queue
//       //string and clean upp the new_shot string.
//       if(appData == '}'){
//         shooting_queue += new_shot;
//         new_shot = "";
//         Serial.println("Queue growing larger: " + shooting_queue);
//        }
//     }
// }

// void shoot(){
//   //Here should elon fetch the next ball and bring it to the motors for shooting.
  // Serial.println("Fire! " + String(shot_delay) + " " + left_right + " " + up_down + " " + motor_speed);
  
//   //Resetting the configuration of shots and state telling the loop()
//   //that it is safe to configure the next shot i.e. shooting = false
  // shooting = false;
  // shot_delay = 0;
  // left_right = 0;
  // up_down = 0;
  // motor_speed = 0;
  // previous_time = 0;
// }


// void configure_shot(){

//   //Extract next shot:
//   String delimiter = "}";
//   int pos = shooting_queue.indexOf(delimiter);
//   String shot = shooting_queue.substring(0, pos+1);
//   Serial.println("Shoot " + shot);
  
//   //Delete the shot just extracted from the queue.
//   shooting_queue.remove(0, pos+delimiter.length());

//   //Delete '{' and '}' from the shot string.
//   shot.remove(0,1);
//   shot.remove(shot.length()-1, 1);

//   //Extract the values for the shot: shot_delay, left_right, up_down, motor_speed
  // for(int i = 0; i < 4; i++){
  //   pos = shot.indexOf(',');
  //   int value = (shot.substring(0, pos)).toInt();
  //   shot.remove(0, pos + 1);

  //   switch(i){
  //     case 0:
  //       shot_delay = value;
  //       break;
  //     case 1:
  //       left_right = value;
  //       break;
  //     case 2:
  //       up_down = value;
  //       break;
  //     case 3:
  //       motor_speed = value;
  //       break;
  //     default:
  //       break;
  //     }
  //   }

  // Serial.println("Delay for next shot: " + String(shot_delay));
  
//   //Setting previous time to current time (millis()), used instead of delay(millisec)
//   //because delay blocks the loop() but using millis does not, therefore we can still
//   //receive and parse request from the app.
//   previous_time = millis();
  
//   //Setting the state to shooting, so the loop wont configure the next shot and instead
//   //waits for shooting to become false again (meaning that the shot has been fired) and
//   //it is safe to parse the queue and configure the next shot.
//   shooting = true;
//   }
