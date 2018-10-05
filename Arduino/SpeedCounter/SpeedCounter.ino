


//potenciometro e botoes
int Potnc = A3;
int UpBtn = A5;
int DownBtn = A4;
int LedUp = A1;
int LedDown = A2;


void setup() {                
  // initialize the digital PortD as an output.
  DDRD = B11111111;

  //botoes de pressao
  pinMode(UpBtn, INPUT);
  pinMode(DownBtn, INPUT);
  pinMode(LedUp, OUTPUT);
  pinMode(LedDown, OUTPUT); 
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

void loop(){
  
  // variaveis de contagem
  boolean Up = false;
  boolean Down = false;
  int time;
  int valor = 0;
  
  while(true){
    Up = false;
    Down = false;
    digitalWrite(LedUp,LOW);
    digitalWrite(LedDown,LOW);
  while(Up == false && Down == false){
    
    PORTD = B01111111;
    
    if (digitalRead(UpBtn) == HIGH){
      Up = true;
      digitalWrite(LedUp,HIGH);
      PORTD = B11111101;
      delay(1000);
      
    }
    if (digitalRead(DownBtn) == HIGH){
      Down = true;
      digitalWrite(LedDown,HIGH);
      PORTD = B11101111;
      delay(1000);
      
    }
  }
  
  while(digitalRead(UpBtn) != HIGH && digitalRead(DownBtn) != HIGH){
    time = (analogRead(Potnc)*9)/1000;
    Mostra(time);
  }
  
  // contagem
  for(Up?valor = 0: valor = 9; Up?valor <=9:valor >=0; Up?valor++:valor--){
    Up?digitalWrite(LedUp,HIGH):digitalWrite(LedDown,HIGH);
    Mostra(valor);
    delay(time*500);
    digitalWrite(LedUp,LOW);
    digitalWrite(LedDown,LOW);
    delay(time*500);
  }
  }
}
