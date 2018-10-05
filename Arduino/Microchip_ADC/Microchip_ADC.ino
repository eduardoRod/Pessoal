/*
Código para leitura dos canais de ADC
Microchip MCP3008 

*** pins ***
pin 13 - CLCK
pin 12 - MOSI
pin 11 - MISO
pin 10 - CS
*/
#include <SPI.h>
#define CS 10

uint8_t codCanal[8] ={
 0x80 , 0x90 , 0xA0 , 0xB0,
 0xC0 , 0xD0 , 0xE0 , 0xF0};

int i,j,k;

void setup(){
  pinMode(CS,OUTPUT);
  digitalWrite(CS,HIGH);
  Serial.begin(115200);
  SPI.begin();
  SPI.setDataMode(1);
}

long readADC(int canal){
  byte inByte = 0;
  long valor;

  digitalWrite(CS,LOW);
  SPI.transfer(0x01);
  valor = SPI.transfer(codCanal[canal]);
  valor &= 0b00000111;
  valor = valor << 8;
  inByte = SPI.transfer(0x00);
  valor = valor | inByte;
  digitalWrite(CS,HIGH);
  
  return valor;
}

void loop(){
  
}
