
/*
 * //LEFT -> 1411
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */ 

//RFID thingies
#include <SPI.h>
#include <MFRC522.h>

//Servo thingies
#include <Servo.h>

//LCD Thingies
#include <LiquidCrystal.h>

 //RFID: 3.3v, gnd,  53 52 51 50 49 
 //Servo: 5v, gnd, 7
 //ldr: A0
 //LCD: 31,33,35,37,39,41

//Declare servo
Servo myservo;
int servoPin = 7;
boolean openDoor = false;

//Declare rfid
constexpr uint8_t RST_PIN = 49;     //change this   
constexpr uint8_t SS_PIN = 53;    //change this    
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

//Declare lcd
int rs = 41, en = 39, d4 = 37, d5 = 35, d6 = 33, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//declare ldr
int ldrPin = A0;
int ldrVal = 0;
boolean waitingCard = false;
boolean beepNoisesFlag = false;
boolean cardInserted = false;
long cardTimerCounter = 0;

//Range values 
int minVal = 255;
int maxVal = 0;
int midVal = 0;

//Time thingies
unsigned long currentTime;

void setup() {
  //Start serial
  Serial.begin(9600);
  //Start rfid thingies   
  SPI.begin();      
  mfrc522.PCD_Init();
  //Servo thingies 
  myservo.attach(servoPin);  
  //HODOR
  myservo.write(0);
  //LCD thingies
  lcd.begin(16, 2);
  //LDR thingies
  pinMode(ldrPin,INPUT);
  calibrateLDR();
  //Builtin led for calibartion purposes
  pinMode(13,OUTPUT);

  lcd.clear();
  lcd.print("Esperando tarjeta");
    
}


void calibrateLDR(){
  lcd.print("Calibrando...");
  currentTime = millis();
  int val;
  digitalWrite(13,HIGH);
  //Four seconds to get min val
  while( millis() < (currentTime + 6000) ){
    val = analogRead(ldrPin);
    if(val < minVal){
      minVal = val;
    }
    if(val > maxVal){
      maxVal = val;
    }
  }
  //Serial.println("Min: " + String(minVal));
  lcd.clear();
  lcd.print("Min: " + String(minVal));
  lcd.setCursor(0,1);
  lcd.print("Max: " + String(maxVal));
  midVal = (maxVal - minVal)/2;
  delay(3000);
  lcd.clear();
  lcd.print("LDR Calibrado");
  digitalWrite(13,LOW);
}


String getUid(){
  if(  mfrc522.PICC_IsNewCardPresent()   && mfrc522.PICC_ReadCardSerial() ){

      String res = "";
      // Dump debug info about the card; PICC_HaltA() is automatically called
      //Serial.println("UID MANUALLY: ");
      for(int i = 0; i < mfrc522.uid.size;i++){
            if(mfrc522.uid.uidByte[i] < 0x10){
              res = res + "0";
            }
            res = res + String(mfrc522.uid.uidByte[i], HEX);
            if(i != mfrc522.uid.size -1)
              res = res + ":";
       }
        mfrc522.PICC_HaltA();         // <---- halts reading
        return res;
  
   }
   else{
      return "";
   }
}


char c;

void loop() {
  //Check rfid
  String res = getUid();
  if(res != ""){
    
    lcd.clear();
    lcd.print("Sending request");
    Serial.println(res); //DO NOT DELETE THIS NEVER
     while(Serial.available() == 0){
      //Waits for response
     }
     delay(100); //wait to be written
     c = Serial.read();
     if(c == 't'){
        myservo.write(90);
        currentTime = millis();
        openDoor = true;
        waitingCard  = true;
        beepNoisesFlag = true;
        lcd.clear();
        lcd.print("Bienvenido!");
     }
  }
  
  //Check ldr
  if(waitingCard){
      if( millis() >= currentTime + 10000){
          Serial.println("Card delete");
          waitingCard = false;

          //REMOVE
          lcd.clear();
          lcd.print("Reserva borrada!");
      }
      else if( millis() >= (currentTime + 5000) && beepNoisesFlag){
          beepNoisesFlag = false;
          Serial.println("Card beeps");

          //REMOVE
          lcd.clear();
          lcd.print("Inserta tarjeta!");
      }
      ldrVal = analogRead(ldrPin);
      //if ldrVal is less than the threshold, deny the flag and print "cart inserted"
      if(ldrVal <= (minVal+(midVal/2))){ //means card WAS inserted
          waitingCard = false; //we stop the code from checking this flag
          cardInserted = true;  //and we tell it to check this  one

          //REMOVE
          lcd.clear();
          lcd.print("Tarjeta insertada!");
          lcd.setCursor(0,1);
          lcd.print("Bienvenido!");
          Serial.println("Card inserted");
      }  
  }
  
  //Check servo 
  if(openDoor){
    if( millis() >= (currentTime + 5000)  ){
      openDoor = false;
      myservo.write(0);
    }
  }
  if(cardInserted){ //if the card is inserted, we'll be checking if the card is removed
    ldrVal = analogRead(ldrPin);
    if(ldrVal >= (maxVal-(midVal))){ //means the card was removed
        cardInserted = false;
        Serial.println("Card removed");
        
        //REMOVE
          lcd.clear();
          lcd.print("Removed!");
    } 
  }
}
