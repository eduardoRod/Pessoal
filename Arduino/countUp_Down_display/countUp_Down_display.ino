void setup() {                
  // initialize the digital PortD as an output.
  DDRD = B11111111; 
}

void Mostra(int digito){
  
  switch (digito){
    case 0: PORTD = B10000001; break;
    case 1: PORTD = B11110011; break;
    case 2: PORTD = B01001001; break;
    case 3: PORTD = B01100001; break;
    case 4: PORTD = B00110011; break;
    case 5: PORTD = B00100101; break;
    case 6: PORTD = B00000111; break;
    case 7: PORTD = B11110001; break;
    case 8: PORTD = B00000001; break;
    case 9: PORTD = B00110001; break;
    default: PORTD = B01111111;
  }
}

void loop() {
  
  int valor;
  boolean up=true;
  
  for (valor = 0; 1; up?valor++:valor--){
    Mostra(valor);
    delay(500);
    if(valor==9) up=0;
    else if(valor==0) up=1;
  }

}
