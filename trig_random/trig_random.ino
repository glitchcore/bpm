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
  bool level = digitalRead(in);
  digitalWrite(synth_out, level);
  digitalWrite(trig_out, level);
  digitalWrite(led, level);

  Serial.print(1024 - analogRead(A2)); Serial.print(" ");
  Serial.print(1024 - analogRead(A3)); Serial.print(" ");
  Serial.println();
}
