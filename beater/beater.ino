const int STEPS_PAD = 5;
const int STEPS = 400;

#include "beater.hpp"
#include "sequencer.hpp"

int led = 13;

int en_0 = 3;
int trig = 9;

Beater beat0;
Sequencer seq0;

/*Beater beat1;
Sequencer seq1;*/

void setup() {
    beat0.step = 5;
    beat0.dir = 6;
    beat0.step_target = STEPS;
    beat0.step_delay = 300;

    /*beat1.step = 6;
    beat1.dir = 3;
    beat1.step_target = STEPS;
    beat1.step_delay = 400;*/

    randomSeed(analogRead(0));
    
    pinMode(en_0, OUTPUT);
    pinMode(led, OUTPUT);

    pinMode(trig, INPUT_PULLUP);

    // break the stick
    digitalWrite(en_0, HIGH);
    delay(500);
    digitalWrite(en_0, LOW);
    delay(500);

    init_beater(&beat0);
    // init_beater(&beat1);

    init_sequencer(&seq0);
    // init_sequencer(&seq1);
    
    seq0.beater = &beat0;
    // seq1.beater = &beat1;
    seq0.period = 1000; // 400000;
    // seq1.period = 600000;
    seq0.trig = trig;
}

void loop() {
    update_sequencer(&seq0, micros());
    // update_sequencer(&seq1, micros());

    update_beater(&beat0, micros());
    // update_beater(&beat1, micros());
}
