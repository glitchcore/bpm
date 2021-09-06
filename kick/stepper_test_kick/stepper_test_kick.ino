/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
*/

const int stp1 =  5;
const int stp2 =  6;
const int stp3 =  7;

const int en1 =  8; //was 9

const int dir1 =  2;
const int dir2 =  3;
const int dir3 =  4;


int ledState, ledState2, ledState3 = LOW;
int dirState, dirState2, dirState3 = LOW;

int maxcnt = 1300; //1800 2100 1000
int back =  50; //1800 2100

int del = 20; //50 40 20 14

int but = 0;

#define BUTTON_PIN 11

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  //  pinMode(led, OUTPUT);

  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);

  pinMode(stp1, OUTPUT);
  for (int i = 2; i <= 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.begin(115200);
  digitalWrite(en1, HIGH);
  //delay(2000);

  digitalWrite(en1, HIGH);



  pinMode(BUTTON_PIN, INPUT_PULLUP);


}



// the loop routine runs over and over again forever:
void loop() {
  but = digitalRead(11);

  if (but == LOW) {
    digitalWrite(en1, HIGH);
    delay(50);
    digitalWrite(dir3, HIGH);
    delay(10);

    for (int i = 0; i <= maxcnt; i++) {
      digitalWrite(stp3, HIGH);
      delayMicroseconds(del);               // wait for a second
      digitalWrite(stp3, LOW);
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

    digitalWrite(en1, LOW);
    delay(30);

  }
}
