
unsigned long LAST_UPDT;
const int MAX_DELAY = 20;

void setup(){
  for(int i = 0; i <= 13; i++){
    pinMode(i,OUTPUT);
  }
}

int updateServo(int pin, int pos){
  
  int us = ((pos*750)/180)+1250;
  
  if(millis() - LAST_UPDT >= MAX_DELAY){
    digitalWrite(pin,HIGH);
    delayMicroseconds(us);
    digitalWrite(pin,LOW);
    LAST_UPDT = millis();
  }
}

void loop(){
  int pos1;
  
}
