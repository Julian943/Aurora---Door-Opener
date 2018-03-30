 #include <SevSeg.h>
//Neded:
  //RGB: ground, pin : 10 9 8 
  //1s4d: 31-41
  //no usar el 1 y 0
  //Buzzer: 8
// Arduino Mega, it works on pins 2 - 13 and 44 - 46 <-- analog

int red = 4;
int green = 3;
int blue = 2;

int buzzer = 8;

bool threeNoisesFlag = false;
long threeNoisesTimeMeasure = 0;
int threeNoisesSwitch = 0;

bool passedNoiseFlag = false;
long passedNoiseTimeMeasure = 0;

bool rejectedNoiseFlag = false;
long rejectedNoiseTimeMeasure = 0;

SevSeg sevseg;
long firstTimeMeasure = 0;
bool timerFlag = false;
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
  byte numDigits = 4;   
  byte digitPins[] = {31,37,39,40};
  byte segmentPins[] = {33,41,36,32,30,35,38,34};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setNumber(12312312,2); 
  //Give it a couple of seconds for the script to detect all
  delay(2000);
   
}

void turnOnLedRGB(int redVal, int greenVal, int blueVal){
  analogWrite(red,redVal);
  analogWrite(green,greenVal);
  analogWrite(blue,blueVal);
}

long writeTime(long startTime, long minutes, long seconds){
  double maxTiem = (minutes*60*1000)+(seconds*1000) + 1000;
  double current = (double)millis() - (double)firstTimeMeasure;
  double val = maxTiem - current; //if this fella is zero, time's up!
  
  double resMinutes = (long)(val/60000);
  double resSeconds = (val - (resMinutes*60*1000))/1000;

  long res = resMinutes*100 + resSeconds;
  if(res <= 0){
    sevseg.setNumber(0,2); 
    return 0;
   }
    
  else{
    sevseg.setNumber(res,2); 
    return res;
    }
}

void threeNoisesFunction(){
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

void passedSound(){
   if(millis() > (passedNoiseTimeMeasure + 500)){
      noTone(buzzer); 
      turnOnLedRGB(0,0,0);
   }
   else if(millis() > (passedNoiseTimeMeasure + 100)){
      analogWrite(buzzer,100);
   }
   else if(millis() > (passedNoiseTimeMeasure)){
      analogWrite(buzzer,60);
      turnOnLedRGB(0,0,20);
   }
}

void rejectedSound(){
   if(millis() > (rejectedNoiseTimeMeasure + 500)){
      noTone(buzzer); 
      turnOnLedRGB(0,0,0);
   }
   else if(millis() > (rejectedNoiseTimeMeasure + 100)){
      analogWrite(buzzer,60);
   }
   else if(millis() > (rejectedNoiseTimeMeasure)){
      analogWrite(buzzer,100);
      turnOnLedRGB(255,0,0);
   }
}

char c = "!";
void loop() {
  if(Serial.available()){
    c = Serial.read();
    switch(c){
      case 'r':
        //turns led to red. "sala ocupada"     
        turnOnLedRGB(255,0,0);
      break;
      case 'g':
        //turns led to green. "sala libre"      
        turnOnLedRGB(0,0,20);
       
      break;
      case 'y':
        //turns led to yellow. "sala esperando" 
        turnOnLedRGB(10,0,255);
        
      break;
      case 't':
        //makes three noises with zumbador.  "sala será ocupada" 
        threeNoisesSwitch = 0;
        threeNoisesFlag = true;
    
      break;
      case 'a':
        //turns on buzzer. lcd indicates "la sala será evacuada" 
        tone(buzzer,500);
      break;
      case 's':
        //turns off buzzer. lcd indicates "sala evacuada"
        noTone(buzzer);
        break; 
      case 'c':
        timerFlag = true;
        firstTimeMeasure = millis();
      case 'p':
        passedNoiseTimeMeasure = millis();
        passedNoiseFlag = true;
        //blinks green..er. makes success noise with zumbador. lcd indicates "bienvenido!" 
      break;
      case 'n':
        //blinks red...er. makes failure noise with zumbador. lcd indicates "no reservaste la sala" 
        rejectedNoiseTimeMeasure = millis();
        rejectedNoiseFlag = true;
      break;
    }
  }

  if(threeNoisesFlag){
     threeNoisesFunction();
     if(threeNoisesSwitch >=7){ //7 makes three noises...dunno why, just leave it
          threeNoisesFlag = false;
      }
   }

  if(timerFlag){
    if(writeTime(firstTimeMeasure,30,0) == 0){
      timerFlag  = false;
      tone(buzzer,500);
    }
      
  }

  if(passedNoiseFlag){
    passedSound();
    if(millis() > (passedNoiseTimeMeasure + 2000)){
      passedNoiseFlag = false;
   }
  }

  if(rejectedNoiseFlag){
     rejectedSound();
     if(millis() > (rejectedNoiseTimeMeasure + 2000)){
        rejectedNoiseFlag = false;
     }
  }

  sevseg.refreshDisplay();
}
