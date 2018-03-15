#include <LiquidCrystal.h>
//Neded:
  //RGB: ground, pin : 10 9 8 
  //LCD: LiquidCrystal lcd(12, 11, 1, 4, 3, 2);
  //Buzzer: 6

LiquidCrystal lcd(12, 11, 1, 4, 3, 2);
int red = 10;
int green = 9;
int blue = 6;
int buzzer = 5;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  //lcd.clear();

  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void turnOnLedRGB(int redVal, int greenVal, int blueVal){
  analogWrite(red,redVal);
  analogWrite(green,greenVal);
  analogWrite(blue,blueVal);
}

char c = "!";
void loop() {
  while(Serial.available() == 0){
    //Waits for response
  }
  c = Serial.read();
  switch(c){
    case 'r':
      //turns led to red.    lcd indicates "sala ocupada"     
      lcd.print("Sala ocupada");
      turnOnLedRGB(255,0,0);
    break;
    case 'g':
      //turns led to green.  lcd indicates "sala libre"      
      lcd.print("Sala libre");
      turnOnLedRGB(0,255,0);
    break;
    case 'y':
      //turns led to yellow. lcd indicates "sala esperando" 
      lcd.print("Sala esperando");
      turnOnLedRGB(255,255,0);
    break;
    case 't':
      //makes three noises with zumbador. lcd indicates "sala será ocupada" 
      lcd.print("La sala sera ocupada");
      for(int i = 0; i < 3; i++){
        analogWrite(buzzer,100);
        delay(500);
        analogWrite(buzzer,0);
        delay(500);
      }
    break;
    case 'a':
      //turns on buzzer. lcd indicates "la sala será evacuada" 
      lcd.print("La sala sera evacuada");
      analogWrite(buzzer,100);
    break;
    case 's':
      //turns off buzzer. lcd indicates "sala evacuada"
      lcd.print("Sala evacuada");
      analogWrite(buzzer,0);
    break;
    case 'p':
      //blinks green..er. makes success noise with zumbador. lcd indicates "bienvenido!" 
      lcd.print("Bienvenido!");
      turnOnLedRGB(173,255,47);
      analogWrite(buzzer,60);
      delay(100);
      analogWrite(buzzer,100);
      delay(500);
      analogWrite(buzzer,0);
      turnOnLedRGB(255,0,0);
    break;
    case 'n':
      //blinks red...er. makes failure noise with zumbador. lcd indicates "no reservaste la sala" 
      lcd.print("No reservaste la sala");
      turnOnLedRGB(220,20,60);
      analogWrite(buzzer,100);
      delay(100);
      analogWrite(buzzer,60);
      delay(500);
      analogWrite(buzzer,0);
      turnOnLedRGB(255,0,0);
    break;
  }
}
