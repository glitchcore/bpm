const int stp =  A5;
const int en =  A0;
const int dir =  A1;

const int pullup = A4;

int ledState = LOW;
int dirState = LOW;

const int led = 13;

int maxcnt = 1300; //1800 2100 1000
int back =  50; //1800 2100

int del = 20; //50 40 20 14

const int BUTTON_PIN = 9;

void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(stp, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(pullup, OUTPUT);
  digitalWrite(pullup, HIGH);
  
  
  digitalWrite(dir, LOW);
  
  Serial.begin(115200);
  digitalWrite(en, HIGH);
  //delay(2000);
}



// the loop routine runs over and over again forever:
void loop() {
  int but = digitalRead(BUTTON_PIN);

  if (but == HIGH) {
    digitalWrite(led, HIGH);
    digitalWrite(en, HIGH);
    delay(50);
    digitalWrite(dir, HIGH);
    delay(10);

    for (int i = 0; i <= maxcnt; i++) {
      digitalWrite(stp, HIGH);
      delayMicroseconds(del);               // wait for a second
      digitalWrite(stp, LOW);
      delayMicroseconds(30);               // wait for a second
    }
//    digitalWrite(dir3, LOW);
//    delay(20);
//    for (int i = 0; i <= maxcnt - back; i++) {
//      //      digitalWrite(stp1, HIGH);
//      digitalWrite(stp3, HIGH);
//      delayMicroseconds(del * 3);               // wait for a second
//      digitalWrite(stp3, LOW);
//      delayMicroseconds(30);               // wait for a second
//    }
    //delay (4);

    digitalWrite(en, LOW);
    delay(30);
    digitalWrite(led, LOW);
  }
}
