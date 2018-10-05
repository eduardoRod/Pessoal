#include<Adafruit_GFX.h>
#include<Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 3, 4);
long int lastMillis = 0;
int i = 0;

void setup(){
  pinMode(2,INPUT);
  digitalWrite(2,HIGH);
  Serial.begin(9600);
  
  display.begin();
  display.setContrast(55);
  display.setTextSize(1);
  display.setTextColor(BLACK);
}

void loop(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("btn = ");
  display.println(digitalRead(2));
  display.print("analog1 = ");
  display.println(analogRead(A0));
  display.print("analog2 = ");
  display.println(analogRead(A2));
  
  if(millis() - lastMillis >= 1000){
    lastMillis = millis();
    if(i == 0){
      i = 20;
    }else{
      i --;
    }
  }
  
  display.setCursor(40,30);
  display.print(i);
  display.display();
  delay(5);
}
