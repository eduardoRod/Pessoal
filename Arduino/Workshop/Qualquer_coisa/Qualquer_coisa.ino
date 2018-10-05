const int ledR = 11;
const int ledY = 12;
const int ledG = 13;
const int btn = 7;
int leitura;
int ultimaLeitura = LOW;
long tmpUltimaLeitura = 0;
int estado = 1;
long mudanca = 0;
long lastPrint = 0;

void setup(){
  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(ledY,OUTPUT);
  pinMode(btn,INPUT_PULLUP);
  Serial.begin(9600);
}

void Luz(int estado){

  switch(estado){
    case 1:
      digitalWrite(ledG,HIGH);
      digitalWrite(ledY,LOW);
      digitalWrite(ledR,LOW);
      mudanca = millis();
      break;
    case 2:
      digitalWrite(ledG,LOW);
      digitalWrite(ledY,HIGH);
      digitalWrite(ledR,LOW);
      mudanca = millis();
      break;
    case 3:
      digitalWrite(ledG,LOW);
      digitalWrite(ledY,LOW);
      digitalWrite(ledR,HIGH);
      mudanca = millis();
      break;
    case 4:
      if(millis() - mudanca >= 1000){
        if(digitalRead(ledG)){
          digitalWrite(ledG,LOW);
          digitalWrite(ledY,HIGH);
        }else if(digitalRead(ledY)){
          digitalWrite(ledY,LOW);
          digitalWrite(ledR,HIGH);
        }else if(digitalRead(ledR)){
          digitalWrite(ledR,LOW);
          digitalWrite(ledG,HIGH);
        }else{
          digitalWrite(ledG,HIGH);
          digitalWrite(ledY,LOW);
          digitalWrite(ledR,LOW);
        }
        mudanca = millis();
      }
  }
}

void Imprime(){
  if(millis() - lastPrint >= 1000){
    Serial.print(digitalRead(btn));
    Serial.print(" estado = ");
    Serial.println(estado);
    Serial.print("Green = ");
    Serial.print(digitalRead(ledG));
    Serial.print("Yellow = ");
    Serial.print(digitalRead(ledY));
    Serial.print("Red = ");
    Serial.println(digitalRead(ledR));
    lastPrint = millis();
  }
}

void loop(){
  
  
  if(ultimaLeitura != leitura){
      ultimaLeitura = leitura;
  }
    
  if (millis() - tmpUltimaLeitura >= 100){
    leitura = digitalRead(btn);
    
    if(leitura == LOW){   
      if(estado == 1){
        estado = 2;
      }else if(estado == 2){
        estado = 3;
      }else if(estado == 3){
        estado = 4;
      }else if(estado == 4){
        estado = 1;
      }
    }
    tmpUltimaLeitura = millis();
  }
  
  Luz(estado);
  
  Imprime();

}
