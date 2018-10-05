#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define CS 10
#define btn1 2
#define btn2 3
#define btn3 4
#define btn4 5

int centroXY[] ={
  21 , 12 , 63 , 12 , 21 , 36 , 63 , 36};

uint8_t codCanal[8] ={
 0x80 , 0x90 , 0xA0 , 0xB0,
 0xC0 , 0xD0 , 0xE0 , 0xF0};

/*
Inicialização de ecrãn 84x48 

*** pins ***
pin A0 - 
*/
Adafruit_PCD8544 display = Adafruit_PCD8544(A0, 6, 7, 8, 9);
int i,j,k;

void setup(){
  pinMode(CS,OUTPUT);
  pinMode(btn1,INPUT);
  pinMode(btn2,INPUT);
  pinMode(btn3,INPUT);
  pinMode(btn4,INPUT);
  digitalWrite(btn1,HIGH);
  digitalWrite(btn2,HIGH);
  digitalWrite(btn3,HIGH);
  digitalWrite(btn4,HIGH);
  digitalWrite(CS,HIGH);
  Serial.begin(115200);
  SPI.begin();
  SPI.setDataMode(1);
  
  display.begin();
  display.setContrast(55);
  display.display();
  delay(2000);
  display.clearDisplay();
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

void drawJoys(){
  
  int x, y;
  
  display.clearDisplay();
  display.drawLine(0, (display.height()/2), display.width(), (display.height()/2), BLACK);
  display.drawLine((display.width()/2), 0, (display.width()/2), display.height(), BLACK);

  for( int i = 0; i < 7; i = i+2){
    y = readADC(i);
    y = map(y, 0, 1024, 10, -12);
    x = readADC(i+1);
    x = map(x, 0, 1024, 19, -21);
    display.drawPixel(centroXY[i] + x, centroXY[i+1] + y, BLACK);
    display.drawPixel(centroXY[i] + x, centroXY[i+1] + y+1, BLACK);
    display.drawPixel(centroXY[i] + x+1, centroXY[i+1] + y, BLACK);
    display.drawPixel(centroXY[i] + x+1, centroXY[i+1] + y+1, BLACK);
  }
  
  display.display();
  
}

void loop(){
  drawJoys();
}
