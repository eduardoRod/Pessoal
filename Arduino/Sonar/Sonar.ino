int echo_pin = 23;
int trigger_pin = 22;

unsigned long time = 0;
int dist;

void setup(){
  Serial.begin(9600);
  pinMode(echo_pin,INPUT);
  pinMode(trigger_pin,OUTPUT);
}

int getDist(){
  digitalWrite(trigger_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin,LOW);
  
  while(digitalRead(echo_pin) == LOW){
  }
  
  time = micros();
  
  while(digitalRead(echo_pin) == HIGH){
  }
  
  time = micros()-time;
  
  return time/58;
}

void loop(){
  dist = getDist();
  Serial.print("Dist = ");
  Serial.println(dist);
  delay(100);
}
