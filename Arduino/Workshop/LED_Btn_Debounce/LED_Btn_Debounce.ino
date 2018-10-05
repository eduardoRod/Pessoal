int led = 13;
int btn = 7;
int leitura;
int ultimaLeitura = LOW;
int tmpUltimaLeitura = 0;
int estado = LOW;

void setup(){
  pinMode(led,OUTPUT);
  pinMode(btn,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){
  
  
  if(ultimaLeitura != leitura){
      ultimaLeitura = leitura;
  }
    
  if (millis() - tmpUltimaLeitura >= 100){
    leitura = digitalRead(btn);
    
    if(leitura == LOW){   
      if(estado == LOW){
        estado = HIGH;
      }else{
        estado = LOW;
      }
    }
    tmpUltimaLeitura = millis();
  }
  
  digitalWrite(led,estado);
  
  Serial.print(digitalRead(btn));
  Serial.print(" estado = ");
  Serial.println(estado);
}
