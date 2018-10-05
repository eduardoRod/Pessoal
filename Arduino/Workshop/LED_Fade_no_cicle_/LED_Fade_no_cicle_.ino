int led = 11;
const int intervalo = 1;
int estado;
int upDown = HIGH;

void setup(){
  pinMode(led,OUTPUT);
}

void loop(){
  
  upDown?estado++:estado--;
  
  if(estado == 255){
    upDown = LOW;
  }else if(estado == 0){
    upDown = HIGH;
  }
  
  analogWrite(led,estado);
  delay(intervalo);  
}
