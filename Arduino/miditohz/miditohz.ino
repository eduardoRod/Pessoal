const int buzz = 10;

void setup(){
  pinMode(buzz,OUTPUT);
  Serial.begin(9600);
}

unsigned int miditohz(int x){
  return 440 * pow(2, (x - 69) / 12.0) + 0.5;
}

void loop(){
  analogWrite(11,analogRead(A0));
  Serial.println(analogRead(A0));
}
