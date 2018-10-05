int led = 11;
const int intervalo = 1;
int estado;

void setup(){
  pinMode(led,OUTPUT);
}

void loop(){
  
  for(estado = 0; estado < 255; estado++){
    analogWrite(led,estado);
    delay(intervalo);
  }
  
  for(estado = 255; estado > 0; estado--){
    analogWrite(led,estado);
    delay(intervalo);
  }
  
}
