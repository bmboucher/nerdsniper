#include "nerdsniper.h"

u8g2_int_t range_level = 0;
u8g2_int_t nerd_level = 0;
unsigned long last_frame = 0;
unsigned long last_stat_print = 0;
unsigned long n_paint_calls = 0;
unsigned long n_frames = 0;

uint8_t range_phase = 0;
bool inc_nerd = true;

void initAnimation() {
    range_level = random(0, RM_L + 1);
    nerd_level = random(NM_MIN_VAL, NM_H + 1);
    last_frame = millis();
    last_stat_print = millis();
}

void nextFrameTargeting() {
    // update range level
    range_phase++;
    if (range_phase == 2) {
        u8g2_int_t tgt_range_level = random(-RM_LOOSE, RM_L + RM_LOOSE + 1);
        if (tgt_range_level > range_level + RM_STICKY) {
            range_level++;
        } else if (tgt_range_level < range_level - RM_STICKY) {
            range_level--;
        }
        if (range_level < 0) {
            range_level = 0;
        } else if (range_level > RM_L) {
            range_level = RM_L;
        }
        range_phase = 0;
    }

    // update nerd level
    if (inc_nerd) {
        nerd_level++;
    } else {
        nerd_level--;
    }
    if (nerd_level > NM_H) {
        // turn around at top
        nerd_level = NM_H;
        inc_nerd = false;
    } else if (nerd_level < NM_MIN_VAL) {
        // turn around at bottom
        nerd_level = NM_MIN_VAL;
        inc_nerd = true;
    } else {
        // randomly flip direction in between
        // offset makes flip more likely near extremes
        u8g2_int_t offset = inc_nerd ? nerd_level : (NM_H - nerd_level);
        // probability varies from 0.05 (offset = 0) to ~0.1 (offset = NM_H)
        if (random(-offset, 10 * NM_H) < NM_H) {
            inc_nerd = !inc_nerd;
        }
    }
}

void nextFrame() {
    if (targeting_mode) {
        nextFrameTargeting();
    } else {
        updateSnipe();
    }
}

void updateAnimation() {
    n_paint_calls++;
    unsigned long curr_t = millis();
    if (curr_t < last_frame || curr_t >= last_frame + FRAME_DELAY) {
        nextFrame();
        last_frame = curr_t;
        n_frames++;
    }
    curr_t = millis();
    if (curr_t < last_frame || curr_t >= last_stat_print + 1000) {
        Serial.print(n_frames);
        Serial.print('/');
        Serial.println(n_paint_calls);
        n_paint_calls = 0;
        n_frames = 0;
        last_stat_print = curr_t;
    }
}