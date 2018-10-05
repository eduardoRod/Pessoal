// =========================================================
//  Variaveis iniciais
// =========================================================

//Pins de ligação de teclado
const int lin[] = {32,34,36,38};
const int col[] = {40,42,44};
const byte LINHAS = 4, COLUNAS = 3;
// valores para cada tecla
char valor[LINHAS][COLUNAS] = {
     {'1','2','3'},
     {'4','5','6'},
     {'7','8','9'},
     {'*','0','#'}
};
// variável para código
char codigo[] = {'0','0','0','0'};

// =========================================================
//  Setup
// =========================================================
void setup(){
  for(int i = 0; i < 4; i++){
    pinMode(lin[i],OUTPUT);
    digitalWrite(lin[i],HIGH);
  }
  for(int i = 0; i < 3; i++){
    pinMode(col[i],INPUT);
    digitalWrite(col[i],HIGH);
  }
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

// =========================================================
//  Varre o teclado e devolve tecla premida
// =========================================================
char varre(){
  char car = 0;
  for(int i = 0; i < 4; i++){
    digitalWrite(lin[i],LOW);
    for(int j = 0; j < 3; j++){
      if(digitalRead(col[j]) == LOW){
        car = valor[i][j];
        delay(20);
        while(digitalRead(col[j]) == LOW){};
      }
    }
    digitalWrite(lin[i],HIGH);
  }
  return car;
}

// =========================================================
//  Verifica se esta a ser premida a combinação '*','#'
// =========================================================
boolean checkReset(){
  
  digitalWrite(lin[3],LOW);
  if(digitalRead(col[0]) == LOW && digitalRead(col[2]) == LOW){
     return 1;
  }
  digitalWrite(lin[3],HIGH);
  
  return 0;
}

// =========================================================
//  Processo principal
// =========================================================
void loop(){
  boolean r = checkReset();
  char c = varre();
  
  if(c != 0){
    Serial.print("Reset: ");
    Serial.print(r);
    Serial.print(" tecla: ");
    Serial.println(c);
  }
  
  delay(50);
}
