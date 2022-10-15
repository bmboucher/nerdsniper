#include "nerdsniper.h"
#include <Adafruit_NeoPixel.h>

#define LED_UPDATE_PERIOD 50

namespace {
    Adafruit_NeoPixel front_led = Adafruit_NeoPixel(1, FRONT_LED, NEO_GRB + NEO_KHZ400);

    unsigned long sequence_start = 0;
    unsigned long last_led_update = 0;
    uint32_t color = 0;
    uint32_t prev_color = 0;

    void set_color(uint8_t r, uint8_t g, uint8_t b) {
        color = front_led.Color(r,g,b);
    }

    void set_white(uint8_t brightness = 255) {
        set_color(brightness, brightness, brightness);
    }

    void set_black() {
        set_color(0,0,0);
    }

    void do_update() {
        if (color == prev_color) {
            return;
        }

        front_led.setPixelColor(0, color);
        front_led.show();
        prev_color = color;
        last_led_update = millis();
    }
}

void initFrontLED() {
    front_led.begin();
    front_led.setPixelColor(0, 0);
    front_led.show();
}

void startFrontLEDSequence() {
    set_white();
    do_update();
    sequence_start = millis();
}

void updateFrontLED() {
    if (targeting_mode) {
        set_black();
    } else {
        if (led_blink_active) {
            // blink white during the "shot"
            if (led_blink) {
                set_white();
            } else {
                set_black();
            }
        } else {
            // hold red afterwards
            set_color(255, 0, 0);
        }
    }

    unsigned long curr_t = millis();
    if (curr_t < last_led_update || curr_t >= last_led_update + LED_UPDATE_PERIOD) {
        do_update();
    }
}