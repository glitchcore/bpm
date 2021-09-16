int a[7];
int prev1, prev2 = 0;

int trval = 200;
int rtrnd = 100;

int out1 = 5;
int out2 = 9;

int res1, res2 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  digitalWrite(out1, LOW);
  digitalWrite(out2, LOW);
  pinMode(13, OUTPUT);

}

void loop() {
  for (int i = 0; i <= 3; i++) {
    a[i] = analogRead (i);
  }
  a[4] = digitalRead (3);
  a[5] = digitalRead (7);
  // val
  a[1] = map(a[1], 0, 1023, 0, trval);
  a[1] = constrain(a[1], 0, trval);
  a[3] = map(a[3], 0, 1023, 0, trval);
  a[3] = constrain(a[3], 0, trval);
  // rnd
  a[0] = map(a[0], 0, 1023, 0, rtrnd);
  a[0] = constrain(a[0], 0, rtrnd);
  a[2] = map(a[2], 0, 1023, 0, rtrnd);
  a[2] = constrain(a[2], 0, rtrnd);
  //




  if (a[4] == 1 && prev1 == 0) {
    res2 = a[3] + (random(0, a[2]) - a[2] / 2);
    res2 = constrain(res2, 0, trval + rtrnd);
    prev1 = 1;
    if (res2 > 0) {
      digitalWrite(out1, 1);
      digitalWrite(13, 1);

      delayMicroseconds(res2);
      digitalWrite(out1, 0);
      digitalWrite(13, 0);
    }
  }
  if (a[4] == 0) {
    prev1 = 0;
  }

  if (a[5] == 1 && prev2 == 0) {
    res1 = a[1] + (random(0, a[0]) - a[0] / 2);
    res1 = constrain(res1, 0, trval + rtrnd);
    prev2 = 1;
    if (res1 > 0) {
      digitalWrite(out2, 1);
      delayMicroseconds(res1);
      digitalWrite(out2, 0);
    }
  }
  if (a[5] == 0) {
    prev2 = 0;
  }

  Serial.print(res1);
  Serial.print(" ");
  Serial.print(res2);

  Serial.println();
  //  delay(100);
}

