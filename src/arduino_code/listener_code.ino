//Francisco Julián Huerta y Munive  A01323513
//Jorge Alberto Beauregard Braco    A01328439
//Tabatha Tabeli Acosta Pastrana    A01550280
//Begoña Montes Gómez               A01329896

//Fecha: 27 de Abril, 2018.

//Descripcion: Codigo del arduino denominado "listener". Escucha por el puerto serial las instrucciones que reciba del ordenador y realiza acciones basado en ello.

#include <SevSeg.h>

//USB: Right. 
//Serial Port MacOS: 1421. 
//Probable port rasp: ACM1

//RGB Led pins
int green = 4;
int blue = 3;
int red = 2;

//Buzzer pin
int buzzer = 8;

bool threeNoisesFlag = false; //Determines whether the arduino must perform three "beeps" with the buzzer 
long threeNoisesTimeMeasure = 0; //Register the time when the blag was switched to true. Keeps track of passed time to know when it will stop making noises.
int threeNoisesSwitch = 0; //Keeps count of the interval (noise, no noise) in order to count how many beeps to make

bool passedNoiseFlag = false; //Determines whether the arduino must perform a "positive beep" to inform the user that its card was approved
long passedNoiseTimeMeasure = 0; //Keeps track of time to perform the combination of sounds accordingly 

bool rejectedNoiseFlag = false; //Determines whether the arduino must perform a "negative beep" to inform the user that its card was rejected
long rejectedNoiseTimeMeasure = 0;//Keeps track of time to perform the combination of sounds accordingly 

long beepFlag = false;  //Determines whether the arduino must perform a "constant beep" to inform the user that it must deposit the card into the card slot
long beepCounter = 0; //Determines whether the buzzer must make a noise or keep quiet
long beepTimer = 0; //Keeps track of time passed in order to determine when the arduino must start to make noise or to keep quiet

//Four digit seven segment display object declaration: 4d7s display
SevSeg sevseg;
bool timerFlag = false;  //Determines wheter the arduino must start counting a specified amount of time in the 4d7s display
long firstTimeMeasure = 0; //Register the time when the arduino must base a specified reamining amount of time left
long timerSeconds = 0; //Keeps track of the seconds left in the timer
long timerMinutes = 0;//Keeps track of the minutes left in the timer
String readingFromSerial; //Stores the amount of seconds read from the serial port that the arduino must write in the 4d7s display 
long secondsRead = 0; //Turns readingFromSerial from string to int

void setup() {
  //Serial port
  Serial.begin(9600);

  //RGB Setup
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);

  //Buzzer setup
  pinMode(buzzer,OUTPUT);

  //Seven segment four digit display:
  byte numDigits = 4; //Register the number of digits
  byte digitPins[] = {31,37,39,40}; //Stores the pins of the digits in the 4d7s display 
  byte segmentPins[] = {33,41,36,32,30,35,38,34}; //Stores the pins of the segments in each digit
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; 
  //Initialize the 4d7s display
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  
  //Turn rgd led to green
  turnOnLedRGB(0,255,0);

  //Give it a couple of seconds for the script to detect all
  delay(2000); 
}

//Method that turns RGB led into a specified color
void turnOnLedRGB(int redVal, int greenVal, int blueVal){
  analogWrite(red,redVal);
  analogWrite(green,greenVal);
  analogWrite(blue,blueVal);
}

//Method that writes to the 4d7s display.
  //Receives startTime(firstTimeMeasure), minutes to be written and seconds to be written
long writeTime(long startTime, long minutes, long seconds){
  //Turns the minutes and seconds received to milliseconds 
  double maxTiem = (minutes*60*1000)+(seconds*1000) + 1000; //Add 1000 to correct a phasing error
  double current = (double)millis() - (double)startTime; //obtaines the time passed between the first measure of time and the current time. 
  double val = maxTiem - current; //Gets the time that MUST be written in the 4d7s display calculating the time left between the time that must be written(maxTiem) and the current time that has passed(current)
  
  
  double resMinutes = (long)(val/60000); //Calculates  the minutes that must be written to the display
  double resSeconds = (val - (resMinutes*60*1000))/1000;//Calculates  the seconds that must be written to the display

  //Gets the value that should be sent to the 4d7s object
  long res = resMinutes*100 + resSeconds;

  //if there's no time left, write zero
  if(res <= 0){
    sevseg.setNumber(0,2); 
    return 0;
   }
  else{
    sevseg.setNumber(res,2); 
    return res;
    }
}

//Method that makes the buzzer beeps indefinitely
void indefiniteBeeps(){
  //If the counter is even, we make no noise
  if(beepCounter%2 == 0){
    noTone(buzzer);
  }
  //Else, it makes a noise
  else{
    tone(buzzer,500); 
  }

  //Updates the timer when a specified time passes (500 milliseconds)
  if( (millis()) > (beepTimer + 500)){
    beepTimer = millis();
    beepCounter++;
  }
}

//Method that makes the buzzer to make three noiess
void threeNoisesFunction(){ //Same thing as the indefiniteBeeps method
  if(threeNoisesSwitch%2 == 0){
      noTone(buzzer);
   }
   else{
      tone(buzzer,500); 
   }

   if( (millis()) > (threeNoisesTimeMeasure + 500)){
      threeNoisesTimeMeasure = millis();
      threeNoisesSwitch++;
   } 
}

//Makes a passed sound in the buzzer 
void passedSound(){
  //if a big time has passsed, shut the buzzer 
  if(millis() > (passedNoiseTimeMeasure + 500)){
    noTone(buzzer); 
  }
  //if only 100 ms seconds, make a higher frequency noise
  else if(millis() > (passedNoiseTimeMeasure + 100)){
    analogWrite(buzzer,100);
  }
  //if only 100 ms seconds, make a higher frequency noise
  else if(millis() > (passedNoiseTimeMeasure)){
    analogWrite(buzzer,60);
  }
}

//Makes a rejected sound in the buzzer 
void rejectedSound(){
   if(millis() > (rejectedNoiseTimeMeasure + 500)){
      noTone(buzzer); 
   }
   else if(millis() > (rejectedNoiseTimeMeasure + 100)){
      analogWrite(buzzer,60);
   }
   else if(millis() > (rejectedNoiseTimeMeasure)){
      analogWrite(buzzer,100);
   }
}

//char that indicates what to do
char c = "!";
void loop() {
  //The arduino waits for the serial to write something:
  if(Serial.available()){
    c = Serial.read();
    switch(c){
      case 'r':
        //turns led to red. 
        turnOnLedRGB(255,0,0);
      break;
      case 'g':
        //turns led to green
        turnOnLedRGB(0,255,0);
      break;
      case 'y':
        //turns led to yellow.
        turnOnLedRGB(125,125,0);
      break;
      case 't':
        //makes three noises with buzzer
        threeNoisesSwitch = 0; //Resets the counter of the threeNoises count 
        threeNoisesFlag = true; //Sets the flag to true in order to be evaluated
      break;
      case 'b':
        //indefinite beeps!
        beepFlag = true;
        beepCounter = 0;
      break;
      case 'a':
        //turns on buzzer indefinitely
        beepFlag = false;
        tone(buzzer,500);
      break;
      case 'h':
        //stop timer and noise
        timerFlag  = false;
        writeTime(-1,-1,-1);
      case 's':
        //turns off buzzer.
        noTone(buzzer);
        beepFlag = false;
        break; 
      case 'c':
          turnOnLedRGB(255,0,0);
         //read seconds written
        readingFromSerial = "";
        c = Serial.read(); //reads a starting c 
        while(c != 'c'){ //While the serial does not send a finishing c, we read and concatenate digits
            if(c <= '9' && c >= '0'){
                readingFromSerial+=c;   
              }
           c = Serial.read();
         }
         //parses string to integer
         secondsRead = (long)readingFromSerial.toInt();
         Serial.println(readingFromSerial);
         timerSeconds = secondsRead%60;
         timerMinutes = secondsRead/60; 
         firstTimeMeasure = millis();
         timerFlag = true;

        
      case 'p':
        //blinks green..er. makes success noise with buzzer.
        passedNoiseTimeMeasure = millis();
        passedNoiseFlag = true; 
      break;
      case 'n':
        //blinks red...er. makes failure noise with zumbador.
        rejectedNoiseTimeMeasure = millis();
        rejectedNoiseFlag = true;
      break;
    }
  }

  //Now we check for the active flags

  //If the flag is active, call the function.
  if(threeNoisesFlag){
     threeNoisesFunction();
     //If the counter goes to 7, turn of the flag
     if(threeNoisesSwitch >=7){ //7 makes three noises...dunno why, just leave it
          threeNoisesFlag = false;
      }
   }

  //if the beep flag is activve, we mkae indefinite sounds 
  if(beepFlag){
    indefiniteBeeps();
  }

  //If the timerflag is active, we write the current time to the 4d7s display 
  if(timerFlag){
    //If the method returns a zero, it means time has run out.
    if(writeTime(firstTimeMeasure,timerMinutes,timerSeconds) == 0){
      //so we shut down this flag and make the buzzer noise
      timerFlag  = false;
      tone(buzzer,500);
    }
  }

  //if passedNoiseFlag is true, we make the sound 
  if(passedNoiseFlag){
    passedSound();
    //we deactivate the flag when two seconds has passed
    if(millis() > (passedNoiseTimeMeasure + 2000)){
      passedNoiseFlag = false;
   }
  }

  //Same thing 
  if(rejectedNoiseFlag){
     rejectedSound();
     if(millis() > (rejectedNoiseTimeMeasure + 2000)){
        rejectedNoiseFlag = false;
     }
  }

  //Refreshes the display in each iteration
  sevseg.refreshDisplay();
}
