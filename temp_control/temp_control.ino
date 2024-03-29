int thermo = A5;
int heater = 9;
int ext_led = 6;
int led = 13;

float read_temp(int pin) {
  const float R0 = 5.1e3;
  const float B = 3950;
  const float R25 = 10e3;
  const float T_BASE = 23;
  float r2 = R0 * (1./((float)analogRead(pin)/1023.) - 1.);
  float k = log(r2 / R25) / B + 1.0 / (273.15 + T_BASE);
  return 1.0 / k - 273.15;
}
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(heater, OUTPUT);
  pinMode(led, OUTPUT);
}

bool heating = false;
float temp = 20.0;
const float N_temp = 0.02;
const float N_target = 1.0;
float target_temp = 30;

void loop() {
  analogWrite(ext_led, (1024 - analogRead(A2))/4);

  temp = temp * (1. - N_temp) + read_temp(A5) * N_temp;
  float target_raw = 1. - (float)analogRead(A3)/1024.;
  float target_map = target_raw * (35 - 15) + 15;
  
  target_temp = target_temp * (1. - N_target) + target_map * N_target;

  if(heating) {
    if(temp > target_temp + 0.1) {
      heating = false;
    }
  } else {
    if(temp < target_temp - 0.1) {
      heating = true;
    }
  }

  digitalWrite(led, heating);
  digitalWrite(heater, heating);

  // Serial.print(1024 - analogRead(A2)); Serial.print(" ");
  // Serial.print(1024 - analogRead(A3)); Serial.print(" ");
  Serial.print("{");
  Serial.print("\"target_temp\":");Serial.print(target_temp);Serial.print(",");
  Serial.print("\"heating\":");Serial.print(heating);Serial.print(",");
  Serial.print("\"temp\":");Serial.print(temp);
  Serial.print("}");Serial.println();

  delay(100);
}
