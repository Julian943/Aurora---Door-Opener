#include <Esplora.h> 

void setup() {
    Serial.begin(9600);
    Esplora.writeRGB(1,0,1);
    delay(1000);
    Esplora.writeRGB(0,0,0);
}
char c = "!";
bool buttonFlag = false;
String matricula = "";
void loop() {
    Esplora.writeRGB(1,1,1);
    while(!buttonFlag){
        if (Esplora.readButton(SWITCH_DOWN) == LOW) {
            buttonFlag = true;
            matricula = "A01323513";
        }
        if (Esplora.readButton(SWITCH_UP) == LOW) {
            buttonFlag = true;
            matricula = "A01323512";
        }
        if (Esplora.readButton(SWITCH_LEFT) == LOW) {
            buttonFlag = true;
            matricula = "A01323511";
        }
        if (Esplora.readButton(SWITCH_RIGHT) == LOW) {
            buttonFlag = true;
            matricula = "A01323510";
        }
    }
    Serial.println(matricula);
    buttonFlag = false;
    Esplora.writeRGB(0,0,1);
    while(Serial.available() == 0){
    //Waits for response
    }
    c = Serial.read();
    if(c == 't'){
        Esplora.writeRGB(0,1,0);
        delay(1000);
        Esplora.writeRGB(0, 0, 0); 
    }
    else{
        Esplora.writeRGB(1,0,0);
        delay(1000);
        Esplora.writeRGB(0, 0, 0); 
    }
}
