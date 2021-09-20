int in = 6;
int synth_out = 9;
int trig_out = 5;
int led = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(synth_out, OUTPUT);
  pinMode(trig_out, OUTPUT);
  pinMode(in, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(digitalRead(in) == LOW) {}
  
  int delay_time = (1023 - analogRead(A2)) * 9 + 1000;
  Serial.print("pulse width:"); Serial.println(delay_time);
  
  digitalWrite(synth_out, HIGH);
  digitalWrite(trig_out, HIGH);
  digitalWrite(led, HIGH);
  delayMicroseconds(delay_time);
  digitalWrite(synth_out, LOW);
  digitalWrite(trig_out, LOW);
  digitalWrite(led, LOW);
  
  delay(1);
  
  while(digitalRead(in) == HIGH) {}

  delay(1);
  
  /*Serial.print(); Serial.print(" ");
  Serial.print(1024 - analogRead(A3)); Serial.print(" ");
  Serial.println();*/
}
