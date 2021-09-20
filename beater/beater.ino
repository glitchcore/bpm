const int STEPS_PAD = 5;

// #include "beater.hpp"
// #include "sequencer.hpp"

int led = 13;

#define KICK

#ifdef KICK
const int stp =  A5;
const int en =  A0;
const int dir =  A1;
#else
int en = 3;
int stp = 5;
int dir = 6;
#endif

const int pullup = A4;
int trig = 9;

void setup() {
    pinMode(en, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(dir, OUTPUT);
    
    pinMode(pullup, OUTPUT);
    digitalWrite(pullup, HIGH);

    pinMode(trig, INPUT_PULLUP);

    digitalWrite(en, HIGH);

    #ifdef KICK
    digitalWrite(dir, HIGH);
    #else
    digitalWrite(dir, HIGH);
    #endif

    Serial.begin(115200);
}

void loop() {
  int pulse = pulseIn(trig, HIGH);
  if(pulse == 0 || pulse > 50000) return;
  if(pulse > 10000) pulse = 10000;
  Serial.print("pulse: ");Serial.print(pulse);
  
  #ifdef KICK
  digitalWrite(en, HIGH);
  #else
  digitalWrite(en, LOW);
  #endif
  delay(5);
  
  // push the stick  

  int period_us = 1000;
  int accel_rest = 0;
  int accel_n = 0;
  
  #ifdef KICK
  int target_period = (10000 - pulse)/80 + 0; // force of beat
  const int N = 4;
  const int STEPS = 900;
  #else
  int target_period = (10000 - pulse)/10 + 50; // force of beat
  const int N = 8;
  const int STEPS = 400;
  #endif
  
  Serial.print("period: ");Serial.println(target_period);
  
  
  for(int i = 0; i < STEPS; i++) {
    digitalWrite(stp, HIGH);
    delayMicroseconds(5);
    digitalWrite(stp, LOW);

    delayMicroseconds(period_us > target_period ? period_us : target_period);

    accel_n += 1;
    period_us -= (2 * period_us + accel_rest)/(N * accel_n + 1);
    accel_rest = ((2 * period_us) + accel_rest) % (N * accel_n + 1);
  }
  // free fall
  #ifdef KICK
  digitalWrite(en, LOW);
  #else
  digitalWrite(en, HIGH);
  #endif

  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);

  while(digitalRead(trig) == HIGH) {}
}
