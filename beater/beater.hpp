
typedef enum {
    BeatIdle,
    BeatRun
} BeatState;

typedef struct {
    BeatState state;
    bool step_state;
    bool dir_state; // up = true
    uint32_t next_micros;
    uint16_t step_target;
    uint16_t step_count;
    uint32_t step_delay;

    uint32_t idle_time;

    int step;
    int dir;
} Beater;

void init_beater(Beater* beater) {
    beater->state = BeatRun;
    
    beater->step_state = false;
    beater->dir_state = false;

    beater->next_micros = 0;
    beater->step_count = 0;

    pinMode(beater->step, OUTPUT);
    pinMode(beater->dir, OUTPUT);

    // up
    digitalWrite(beater->dir, LOW);
    for(int i = 0; i < STEPS - STEPS_PAD; i++) {
        digitalWrite(beater->step, HIGH);
        delayMicroseconds(50);
        digitalWrite(beater->step, LOW);

        delayMicroseconds(2000);
    }
    delay(500);
}

void trig_beater(Beater* beater, uint32_t current_micros) {
    beater->state = BeatRun;
    beater->dir_state = false;
    beater->step_state = false;
    beater->next_micros = current_micros + beater->step_delay/2;
}

void update_beater(Beater* beater, uint32_t current_micros) {
    switch(beater->state) {
        case BeatRun:
            if(current_micros < beater->next_micros) return; // time not come
        
            beater->step_state = !beater->step_state;

            if(beater->step_state) {
                beater->step_count++;
                
                if(beater->step_count > (beater->dir_state ? beater->step_target - STEPS_PAD : beater->step_target)) {
                    if(!beater->dir_state) {
                        // down
                        beater->dir_state = true; // go to up
                        beater->next_micros = current_micros + beater->step_delay/2;
                    } else {
                        // up
                        // go to IDLE, ready to go 
                        beater->state = BeatIdle;
                        beater->dir_state = false;
                        beater->step_state = false;
                        // beater->next_micros = current_micros + beater->idle_time;
                    }
                    
                    beater->step_count = 0;
                } else {
                    beater->next_micros = current_micros + beater->step_delay/2;
                }
            } else {
                beater->next_micros = current_micros + beater->step_delay/2;
            }
        break;
    }

    digitalWrite(beater->dir, beater->dir_state ? LOW : HIGH);
    digitalWrite(beater->step, beater->step_state ? HIGH : LOW);
}
