#define Pot A15
#define Slave 53
#define ChB 0x12
#define btnU 32
#define btnD 28
#define MAXVALUE 255

#include <SPI.h>
int value;

void setup(){
  pinMode(Pot,INPUT);
  pinMode(btnU,INPUT);
  digitalWrite(btnU,HIGH);
  pinMode(btnD,INPUT);
  digitalWrite(btnD,HIGH);
  pinMode(Slave,OUTPUT);
  digitalWrite(Slave,HIGH);
  Serial.begin(115200);
  SPI.begin();
  digitalPotWrite(ChB,0);
}

void loop(){
  
  if(digitalRead(btnU) == 0 && value < 255){
    value = value + MAXVALUE/50;
    digitalPotWrite(ChB,value);
    while(digitalRead(btnU) == 0);
    Serial.println(analogRead(Pot));
    delay(10);
  }
  
  if(digitalRead(btnD) == 0 && value > 0){
    value = value - MAXVALUE/50;
    digitalPotWrite(ChB,value);
    while(digitalRead(btnD) == 0);
    Serial.println(analogRead(Pot));
    delay(10);
  }
}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(Slave,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(Slave,HIGH); 
}
