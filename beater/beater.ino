const int STEPS_PAD = 5;

// #include "beater.hpp"
// #include "sequencer.hpp"

int led = 13;

int en = 3;
int trig = 9;
int stp = 5;
int dir = 6;

void setup() {
    pinMode(en, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(dir, OUTPUT);

    pinMode(trig, INPUT_PULLUP);

    digitalWrite(en, HIGH);
}

void loop() {
  while(digitalRead(trig) == LOW) {}

  digitalWrite(en, LOW);
  delay(5);
  // push the stick
  digitalWrite(dir, HIGH);

  int period_us = 1000;
  int accel_rest = 0;
  int accel_n = 0;
  int target_period = 100;
  const int N = 8;
  
  for(int i = 0; i < 300; i++) {
    digitalWrite(stp, HIGH);
    delayMicroseconds(20);
    digitalWrite(stp, LOW);

    delayMicroseconds(period_us > target_period ? period_us : target_period);

    accel_n += 1;
    period_us -= (2 * period_us + accel_rest)/(N * accel_n + 1);
    accel_rest = ((2 * period_us) + accel_rest) % (N * accel_n + 1);
  }
  // free fall
  digitalWrite(en, HIGH);

  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);

  while(digitalRead(trig) == HIGH) {}
}
