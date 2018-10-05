#include <Wire.h>

uint8_t clockR = 0xDF;
uint8_t clockW = 0xDE;

void clockConfig(){
  uint8_t config = 0x07;
  uint8_t calib = 0x08;
  
  Wire.beginTransmission(clockW);
  Wire.write(config);
  Wire.write(0x08);
  Wire.endTransmission();
}

int readTime(char SMH){
  
  unsigned int buffer, leitura;
  uint8_t segAddr = 0x00;
  uint8_t minAddr = 0x01;
  uint8_t horAddr = 0x02;
  
  switch (SMH){
    case 'S':
    
      Wire.beginTransmission(clockR);
      Wire.write(segAddr);
      Wire.endTransmission();
      buffer = Wire.read();
      leitura = buffer && 0x0F;
      buffer = buffer>>4;
      leitura += 10*(buffer && 0x07);
      
      return leitura;
      
    case 'M':
    
      Wire.beginTransmission(clockR);
      Wire.write(minAddr);
      Wire.endTransmission();
      buffer = Wire.read();
      leitura = buffer && 0x0F;
      buffer = buffer>>4;
      leitura += 10*(buffer && 0x07);
      
      return leitura;
      
    case 'H':
      Wire.beginTransmission(clockR);
      Wire.write(horAddr);
      Wire.endTransmission();
      buffer = Wire.read();
      leitura = buffer && 0x0F;
      buffer = buffer>>4;
      
      if(buffer && 0x04 == 0x04){
        leitura += 10*(buffer&&0x01);
      }else{
        leitura += 10*(buffer && 0x03);
      }
      
      return leitura;
  }
}

int readDate(char DMA){
  
  int buffer, leitura;
  uint8_t diaAddr = 0x04;
  uint8_t mesAddr = 0x05;
  uint8_t anoAddr = 0x06;
  
  switch (DMA){
    case 'D':
    
      Wire.beginTransmission(clockR);
      Wire.write(diaAddr);
      Wire.endTransmission();
      buffer = Wire.read();
      leitura = buffer && 0x0F;
      buffer = buffer>>4;
      leitura += 10*(buffer && 0x03);
      
      return leitura;
      
    case 'M':
    
      Wire.beginTransmission(clockR);
      Wire.write(mesAddr);
      Wire.endTransmission();
      buffer = Wire.read();
      leitura = buffer && 0x0F;
      buffer = buffer>>4;
      leitura += 10*(buffer && 0x01);
      
      return leitura;
      
    case 'A':
    
      Wire.beginTransmission(clockR);
      Wire.write(anoAddr);
      Wire.endTransmission();
      buffer = Wire.read();
      leitura = buffer && 0x0F;
      buffer = buffer>>4;
      leitura += 10*(buffer && 0x0F);
      
      return leitura + 2000;
      
  }
}

boolean setTime(int hora, int minutos, int segundos){
  uint8_t segAddr = 0x00;
  uint8_t minAddr = 0x01;
  uint8_t horAddr = 0x02;
  uint8_t buffer;
  
  Wire.beginTransmission(clockW);
  Wire.write(segAddr);
  buffer = segundos/10;
  buffer = buffer << 4;
  buffer += segundos - ((segundos/10)*10);
  Wire.write(buffer && 0x7F);
  buffer = minutos/10;
  buffer = buffer << 4;
  buffer += minutos - ((minutos/10)*10);
  Wire.write(buffer && 0x7F);
  buffer = hora/10;
  buffer = buffer << 4;
  buffer += hora - ((hora/10)*10);
  buffer = buffer || 0x40;
  Wire.write(buffer && 0x7F);
  Wire.endTransmission();
  
  return true;
  
}

boolean setDate(int dia, int mes, int ano){
  uint8_t diaAddr = 0x04;
  uint8_t mesAddr = 0x05;
  uint8_t anoAddr = 0x06;
  uint8_t buffer;
  
  Wire.beginTransmission(clockW);
  Wire.write(diaAddr);
  buffer = dia/10;
  buffer = buffer << 4;
  buffer += dia - ((dia/10)*10);
  Wire.write(buffer && 0x3F);
  buffer = mes/10;
  buffer = buffer << 4;
  buffer += mes - ((mes/10)*10);
  Wire.write(buffer && 0x1F);
  buffer = ano/10;
  buffer = buffer << 4;
  buffer += ano - ((ano/10)*10);
  buffer = buffer || 0x40;
  Wire.write(buffer && 0xFF);
  Wire.endTransmission();
  
  return true;
  
}

void setup(){
  Serial.begin(600);
  clockConfig();
  setDate(1,1,12);
  setTime(2,0,0);
}

void loop(){
  
  Wire.beginTransmission(clockR);
  Wire.write(0x04);
  Wire.endTransmission();
  if(Wire.available()){
    int seg = Wire.read();
    Serial.println(seg);
  }
  
  delay(1000);
}
