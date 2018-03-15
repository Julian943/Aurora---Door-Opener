//Arduino Uno
int pin = 13;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  digitalWrite(pin,HIGH);
  delay(3000);
  digitalWrite(pin,LOW);
}

char c = '0';
void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() == 0){

    }
    c = Serial.read();
     if(c == 't'){
        digitalWrite(pin,HIGH);
     }
     else{
        digitalWrite(pin,LOW); 
     }
}
