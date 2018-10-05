
#include <Wire.h>

void setup(){
  
  Wire.begin();
  
  Wire.beginTransmission(0x40);
  Wire.write(0x04);
  Wire.write(0x41);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x41);
  Wire.write(0x04);
  Wire.write(0x41);
  Wire.endTransmission();
  
}

void refresh(uint8_t addr, uint8_t reg, uint8_t data){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void Scroll(int time){
  uint8_t data[] = {
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x10,
    0x10,
    0x10,
    0x10,
    0x10,
  };
  uint8_t reg1 = 0x0B;
  
  for (int i = 0; i < 4; i++){
    
    for (int j = 0; j < 5; j++){
      refresh(0x40, reg1+j, data[j]);
      refresh(0x41, reg1+j, data[j+5]);
    }
    delay(time);
  
    for (int j = 0; j < 10; j++){
      if (data[j] != 0x40){
        data[j] = data[j] << 2;
      }else{
        data[j] = 0x01;
      }
    }
  }
  
  for (uint8_t adr = 0x0B; adr <= 0x0F; adr++){
    refresh(0x40, adr, 0x00);
  }
  for (uint8_t adr = 0x0B; adr <= 0x0F; adr++){
    refresh(0x41, adr, 0x00);
  }
}

void KitScroll(int time){
  uint8_t add = 0x40;
  uint8_t reg = 0x0B;
  uint8_t data = 0x01;
    
  for(int j = 0; j < 9; j++){
    refresh(add, reg, data);
    delay(time);
  
    if(data == 0x40){
      refresh(add, reg,0x00);
      data = 0x01;
      reg = reg+1;
    }else{
      data = data << 2;
    }
  }
  
  refresh(add, reg, 0x00);
  add = 0x41;
  reg = 0x0B;
  data = 0x01;
  
  for(int j = 0; j < 19; j++){
    refresh(add, reg, data);
    delay(time);
  
    if(data == 0x40){
      refresh(add, reg,0x00);
      data = 0x01;
      reg = reg+1;
    }else{
      data = data << 2;
    }
  }
  
  refresh(add, reg, 0x00);
  add = 0x40;
  reg = 0x0D;
  data = 0x10;
  
  for(int j = 0; j < 9; j++){
    refresh(add, reg, data);
    delay(time);
  
    if(data == 0x40){
      refresh(add, reg,0x00);
      data = 0x01;
      reg = reg+1;
    }else{
      data = data << 2;
    }
  }   
  
  for(int j = 0; j < 9; j++){
    refresh(add, reg, data);
    delay(time);
  
    if(data == 0x01){
      refresh(add, reg,0x00);
      data = 0x40;
      reg = reg-1;
    }else{
      data = data >> 2;
    }
  }
  
  refresh(add, reg, 0x00);
  add = 0x41;
  reg = 0x0F;
  data = 0x40;
  
  for(int j = 0; j < 19; j++){
    refresh(add, reg, data);
    delay(time);
  
    if(data == 0x01){
      refresh(add, reg,0x00);
      data = 0x40;
      reg = reg-1;
    }else{
      data = data >> 2;
    }
  }
  
  refresh(add, reg, 0x00);
  add = 0x40;
  reg = 0x0D;
  data = 0x04;
  
  for(int j = 0; j < 9; j++){
    refresh(add, reg, data);
    delay(time);
  
    if(data == 0x01){
      refresh(add, reg,0x00);
      data = 0x40;
      reg = reg-1;
    }else{
      data = data >> 2;
    }
  }
  
  for (uint8_t adr = 0x0B; adr <= 0x0F; adr++){
    refresh(add, adr, 0x00);
  }
}

void loop(){
  for (int i = 0; i < 6; i++){
    Scroll(50);
  }
  
  for (int i = 0; i < 3; i++){
    KitScroll(15);
  }
}
