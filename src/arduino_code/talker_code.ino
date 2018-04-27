//Francisco Julián Huerta y Munive  A01323513
//Jorge Alberto Beauregard Braco    A01328439
//Tabatha Tabeli Acosta Pastrana    A01550280
//Begoña Montes Gómez               A01329896

//Fecha: 27 de Abril, 2018.

//Descripcion: Codigo del arduino denominado "talker". Detecta intensidad de luz y tarjetas RFID por medio de un sensor LDR y un sensor RFId MFRC522 y se comunica por el ordenador para autorizar o negar la entrada a los usuarios.

//RFID libraries
#include <SPI.h>
#include <MFRC522.h>

//Servo library
#include <Servo.h>

//LCD library
#include <LiquidCrystal.h>
 
//Declare servo
Servo myservo;
int servoPin = 7;
boolean openDoor = false; //Determines whether the door is opened of closed

//Declare rfid
constexpr uint8_t RST_PIN = 49; 
constexpr uint8_t SS_PIN = 53;  
MFRC522 mfrc522(SS_PIN, RST_PIN); 

//Declare lcd
int rs = 41, en = 39, d4 = 37, d5 = 35, d6 = 33, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Declare LDR
int ldrPin = A0;
int ldrVal = 0; //Stores the value measured by LDR 

boolean waitingCard = false; //Determines whether the LDR should expect a card in the slot
boolean beepNoisesFlag = false;  //Determines if the arduinos should indicate to make a beeping noise
boolean cardInserted = false; //Deterines if a card was inserted or not 
long cardTimerCounter = 0; //Counts the time between the approved rfid card and the tolerance time to expect a card in the slot 
int marginValue = 0; //min val that determines if the card was inserted or not

//Range values: Used to calibrate ldr 
int minVal = 255;
int maxVal = 0;
int midVal = 0;

unsigned long currentTime; //Stores current time for general purposes along the code

void setup() {
  //Start serial
  Serial.begin(9600);
  //Start rfid thingies   
  SPI.begin();      
  mfrc522.PCD_Init();
  //Servo thingies 
  myservo.attach(servoPin);  
  //Closes the door initially
  myservo.write(0);
  //LCD thingies
  lcd.begin(16, 2);
  //LDR thingies
  pinMode(ldrPin,INPUT);
  //Calibrates LDR
  calibrateLDR();
  //Clears LCD values
  lcd.clear();
  //Writes to lcd "waiting card"
  lcd.print("Esperando tarjeta");
}


void calibrateLDR(){
  lcd.print("Calibrando...");
  currentTime = millis();
  int val;
  //counts six seconds to calibrate min and max value 
  while( millis() < (currentTime + 6000) ){
    val = analogRead(ldrPin);
    if(val < minVal){
      minVal = val;
    }
    if(val > maxVal){
      maxVal = val;
    }
  }
  //Obtains the mid value between max and min value 
  midVal = (maxVal - minVal)/2;
  //Prints data of the calibrated LDR 
  lcd.clear();
  lcd.print("Min: " + String(minVal) + " Max: " + String(maxVal));
  lcd.setCursor(0,1);
  lcd.print("Mid: " + String(midVal));
  //Waits 3 seconds for the info to be read 
  delay(3000);

  //Clears lcd and indicated that it has finished calibrating the LDR 
  lcd.clear();
  lcd.print("LDR Calibrado");
}

//Gets the unique id of a nfc card 
String getUid(){
  //If the card is nearby and we are able to read something from it:
  if(  mfrc522.PICC_IsNewCardPresent()   && mfrc522.PICC_ReadCardSerial() ){

    String res = "";
    //Reads the unique id 
    for(int i = 0; i < mfrc522.uid.size;i++){
      if(mfrc522.uid.uidByte[i] < 0x10){
        res = res + "0";
      }
      res = res + String(mfrc522.uid.uidByte[i], HEX);
      if(i != mfrc522.uid.size -1)
        res = res + ":";
    }
    //Halts reading of card
    mfrc522.PICC_HaltA();
    return res;

  }
  //If we cannot read or card is not nearby, we return empty string
  else{
    return "";
  }
}

char c;

void loop() {
  //Check rfid
  String res = getUid();
  //If string is not empty, we read something
  if(res != ""){
    //we indicate that we've read something 
    lcd.clear();
    lcd.print("Sending request");
    Serial.println(res); //DO NOT DELETE THIS NEVER
    while(Serial.available() == 0){
      //Waits for response
    }
    delay(100); //wait to be written
    c = Serial.read();
    //If the card is authorized:
    if(c == 't'){
      myservo.write(90); //we open the door 
      currentTime = millis(); //register the time to keep the door open a specified amount of time 
      //activate all kinds of flags
      openDoor = true; 
      waitingCard  = true;
      beepNoisesFlag = true;
      //Clears the lcd and prints "welcome!"
      lcd.clear();
      lcd.print("Bienvenido!");
    }
    else{
      //if car was rejected, we tell the user through the lcd
      lcd.clear();
      lcd.print("Tarjeta");
      lcd.setCursor(0,1);
      lcd.print("rechazada!");
    }
  }
  
  //Check waitingCard flag 
  if(waitingCard){
      //If the card was not inserted in the ldr after ten seconds, we tell the script to delete the reservation
      if( millis() >= currentTime + 10000){
          Serial.println("Card delete");
          waitingCard = false;

          //Tell the user that the reservation was deleted
          lcd.clear();
          lcd.print("Reserva");
          lcd.setCursor(0,1);
          lcd.print("borrada!");
      }
      //If the card has not been inserted after five seconds, we make a beeping noise ONLY ONCE
      else if( millis() >= (currentTime + 5000) && beepNoisesFlag){
          beepNoisesFlag = false;
          Serial.println("Card beeps");
          //Tell the user to insert the card!
          lcd.clear();
          lcd.print("Inserta");
          lcd.setCursor(0,1);
          lcd.print("tarjeta!");
      }
      //Else, we read the ldr value 
      ldrVal = analogRead(ldrPin);
      //if ldrVal is less than the threshold, deny the flag and print "cart inserted"
      if(ldrVal <= (minVal+(midVal/2))){ //means card WAS inserted
          waitingCard = false; //we stop the code from checking this flag
          cardInserted = true;  //and we tell it to check this  one
          marginValue = ldrVal + 20;
          
          //We tell the user that the card was inserted
          lcd.clear();
          lcd.print("Tarjeta insertada!");
          lcd.setCursor(0,1);
          lcd.print("Bienvenido!");
          Serial.println("Card inserted");
      }  
  }
  
  //Check servo 
  if(openDoor){
    //IF the time has already passed, we close the door
    if( millis() >= (currentTime + 5000)  ){
      openDoor = false;
      myservo.write(0);
    }
  }

  //If the card has been inserted, we check the ldr value to see if the vard was removed 
  if(cardInserted){
    ldrVal = analogRead(ldrPin);
    if(ldrVal > marginValue){ //means the card was removed
        cardInserted = false;
        Serial.println("Card removed");
        
        //Tell the user that the card was removed 
          lcd.clear();
          lcd.print("Tarjeta");
          lcd.setCursor(0,1);
          lcd.print("removida!");
    } 
  }
}
