int blob_pin = A0;
int led = 13;
int ext_led = 9;
int trig = 5;
int tune_pin = A1;

typedef enum {
  TrigStateIdle,
  TrigStateRun,
  TrigStateAux,
  TrigStateWait
} TrigState;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(ext_led, OUTPUT);
}

float level_mma = 0.5;
const float MMA = 0.1;

float prev_level = 0.0;
const float LOW_LEVEL = -15.0;
const float HIGH_LEVEL = 5.0;
float ref_level = LOW_LEVEL;

uint32_t prev_t = 0;

uint32_t trig_period = 100000;
uint32_t next_micros = 0;
TrigState trig_state = TrigStateIdle;

void loop() {
  uint32_t t = micros();

  uint32_t aux_trig_period = 1000 + 100000 * (uint32_t)analogRead(tune_pin)/1024;
  
  float level = analogRead(blob_pin) / 1024.0;
  level_mma = level_mma * (1.0 - MMA) + level * MMA;
  float diff_level = (level_mma - prev_level)/((t - prev_t)/1e6) * 50;
  
  prev_level = level_mma;
  prev_t = t;
  
  switch(trig_state) {
    case TrigStateWait:
      if(diff_level > ref_level) {
        ref_level = LOW_LEVEL;
        trig_state = TrigStateIdle;
      }
    break;
    
    case TrigStateIdle:
      if(diff_level < ref_level) {
        ref_level = HIGH_LEVEL;
        next_micros = t + trig_period;
        trig_state = TrigStateRun;
      }
    break;

    case TrigStateRun:
      if(t > next_micros) {
        next_micros = t + aux_trig_period;
        trig_state = TrigStateAux;
      }
    break;

    case TrigStateAux:
      if(t > next_micros) {
        trig_state = TrigStateWait;
      }
    break;
  }

  Serial.print(-100.0); Serial.print(" "); Serial.print(100.0);Serial.print(" ");
  // Serial.print(level_mma * 100); Serial.print(" ");
  Serial.print(diff_level); Serial.print(" ");
  Serial.print(trig_state * 10);  Serial.print(" ");
  // Serial.print(ref_level);  Serial.print(" ");
  Serial.println();

  digitalWrite(led, trig_state == TrigStateRun ? HIGH : LOW);
  digitalWrite(trig, trig_state == TrigStateRun ? HIGH : LOW);
  digitalWrite(ext_led, trig_state == TrigStateRun ? HIGH : LOW);

  delay(1);
}
