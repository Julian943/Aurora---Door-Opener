//Arduino Esplora

#include <Esplora.h> 

void setup() {
  Serial.begin(9600);
  Esplora.writeRGB(255,0,150);
  delay(3000);
  Esplora.writeRGB(0,0,0);
}

char c = '0';
void loop() {
  // put your main code here, to run repeatedly:
    while(Serial.available() == 0){

    }
    c = Serial.read();
    if(c == 't'){
        Esplora.writeRGB(255,0,150);
     }
     else{
        Esplora.writeRGB(0,0,0);
      }
}
