// while() {}
// while(digitalRead(trig) == LOW) {}

typedef enum {
    StateWait,
    StateIdle
}SeqState;

typedef struct {
    SeqState state;
    uint32_t next_micros;
    uint32_t period;

    int trig;

    Beater* beater;
} Sequencer;

void init_sequencer(Sequencer* seq) {
    seq->next_micros = 0;
    seq->state = StateIdle;
}

void update_sequencer(Sequencer* seq, uint32_t current_micros) {
    switch(seq->state) {
        case StateIdle:
            if(seq->trig > 0 && digitalRead(seq->trig) == HIGH) {
                seq->state = StateWait;
                trig_beater(seq->beater, current_micros);
                seq->next_micros = current_micros + seq->period;
            }
        break;

        case StateWait:
            if(seq->trig > 0 && digitalRead(seq->trig) == LOW && current_micros > seq->next_micros) {
                seq->state = StateIdle;
            }
        break;
    }
    
    digitalWrite(13, seq->state == StateIdle ? HIGH : LOW);
}
