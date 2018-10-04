#define DEBUG

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Mag_Address 0x1E    //0011110b, I2C 7bit address of HMC5883
#define Oled_Address 0x3C   //0111100b, I2C 7bit address of display

Adafruit_SSD1306 display(4);

int x,y,z; 

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  display.begin(SSD1306_SWITCHCAPVCC, Oled_Address);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  Wire.begin();
  Wire.beginTransmission(Mag_Address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop() {

  
  updateMagReadings(x,y,z);
  updateDisplay(x,y,z);

  delay(500);
}

void updateMagReadings(int &x, int &y, int &z){
  
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(Mag_Address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  
  Wire.requestFrom(Mag_Address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
}

void updateDisplay(int x, int y, int z){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("x = ");
  display.println(x);
  display.print("y = ");
  display.println(y);
  display.print("z = ");
  display.println(z);
  drawCompass(x,z);
  display.display();
}

void drawCompass(int x, int y){
  int maxVal;
  float auxX;
  float auxY;
  
  if(x < 0){
    auxX = -x;
  }else{
    auxX = x;
  }
  if(y < 0){
    auxY = -y;
  }else{
    auxY = y;
  }

  maxVal = max(auxX,auxY);
  auxX = (x/maxVal)*10;
  auxY = (y/maxVal)*10;
  
  int centerX = (display.width()/4)*3;
  int centerY = display.height()/2;

  display.drawCircle(centerX, centerY, 10, WHITE);
  display.drawLine(centerX, centerY, centerX+auxX, centerY+auxY, WHITE);

  #ifdef DEBUG
    Serial.print("values = ");
    Serial.print(x);
    Serial.print("x , ");
    Serial.print(y);
    Serial.println("y");
    Serial.print("vector = ");
    Serial.print(auxX);
    Serial.print("x , ");
    Serial.print(auxY);
    Serial.println("y");
  #endif
}

