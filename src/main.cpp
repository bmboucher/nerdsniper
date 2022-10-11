#include "nerdsniper.h"

U8G2_SSD1309_128X64_NONAME0_1_4W_HW_SPI screen(U8G2_R0, SCREEN_CS, SCREEN_DC, SCREEN_RST);

unsigned long mark = 0;
bool targeting_mode = true;

void setup() {
    Serial.begin(115200);
    pinMode(TRIGGER, INPUT_PULLUP);
    screen.begin();
    initAnimation();
    mark = millis();
}

void loop() {
    // first, check the trigger state
    if (targeting_mode && millis() - mark > 5000) {
    //if (targeting_mode && digitalRead(TRIGGER) == LOW) {
        targeting_mode = false;
        beginSnipe();
    } else if (!targeting_mode) {
        mark = millis();
    }

    updateAnimation();
    screen.firstPage();
    do {
        if (targeting_mode) {
            drawTargetingScreen();
        } else {
            drawSnipe();
        }
    } while ( screen.nextPage() );
}