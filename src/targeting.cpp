#include "nerdsniper.h"

void crosshair() {
    screen.drawCircle(C_X, C_Y, R);
    screen.drawLine(C_X - RL, C_Y, C_X + RL, C_Y);
    screen.drawLine(C_X, C_Y - RL, C_X, C_Y + RL);
    for (int i = -N_SEGS; i <= N_SEGS; i++) {
    if (i == 0) { continue; }
        const u8g2_int_t tick = (i == N_SEGS || i == -N_SEGS) ? TICK_MAJ : TICK_MIN;
        screen.drawLine(C_X + i * SEG_L, C_Y - tick, C_X + i * SEG_L, C_Y + tick);
        screen.drawLine(C_X - tick, C_Y + i * SEG_L, C_X + tick, C_Y + i * SEG_L);
    }
}

#define N_BATTERY_LEVELS 5

void battery() {
    screen.setFont(u8g2_font_battery19_tn);
    screen.setFontDirection(1);

    const float MIN_BATTERY = 3.3;
    const float MAX_BATTERY = 4.5;
    int relative_battery = int(round(
        N_BATTERY_LEVELS * (battery_level - MIN_BATTERY) / (MAX_BATTERY - MIN_BATTERY)));
    if (relative_battery < 0) { relative_battery = 0; }
    if (relative_battery > N_BATTERY_LEVELS) { relative_battery = N_BATTERY_LEVELS; }
    const char batt_char[1] = {(char)('0' + relative_battery)};

    screen.drawStr(BATTERY_X, BATTERY_Y, batt_char);
}

void rangeMeter() {
    screen.drawLine(RM_MX, RM_H, RM_MX + RM_L, RM_H);
    screen.drawLine(RM_MX + range_level, RM_H - RM_TICK, RM_MX + range_level, RM_H + RM_TICK);
    screen.setFont(u8g2_font_prospero_nbp_tf);
    screen.setFontDirection(0);
    screen.drawStr(RM_MX, RM_TEXT_HEIGHT, "RANGE");
}

const uint8_t DITHER_BITMAP[2] = {
    0b10101100,
    0b11010100
};

void nerdMeter() {
    // draw meter bar
    screen.drawLine(NM_L, NM_T, NM_R, NM_T);
    screen.drawLine(NM_R, NM_T, NM_R, NM_B);
    screen.drawLine(NM_R, NM_B, NM_L, NM_B);
    screen.drawLine(NM_L, NM_B, NM_L, NM_T);
    u8g2_int_t h = NM_MED_T - 1;
    while (h >= NM_MAX_T) {
        screen.drawBitmap(NM_L, h, 1, 2, DITHER_BITMAP);
        h -= 2;
    }
    screen.drawBox(NM_L, NM_T, NM_W, (NM_MAX_T - NM_T + 1));

    // draw marker
    const u8g2_int_t mh = NM_B - nerd_level;
    screen.drawTriangle(
        NM_R + 2, mh, 
        NM_R + 2 + NM_MARKER_W, mh + NM_MARKER_H,
        NM_R + 2 + NM_MARKER_W, mh - NM_MARKER_H);

    // draw label
    screen.setFont(u8g2_font_prospero_nbp_tf);
    screen.setFontDirection(0);
    screen.drawStr(NM_R+2, NM_B, "nerd");
}

void drawTargetingScreen() {
    crosshair();
    battery();
    rangeMeter();
    nerdMeter();
}