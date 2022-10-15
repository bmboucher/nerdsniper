#include "nerdsniper.h"

U8G2_SSD1309_128X64_NONAME0_1_4W_HW_SPI screen(U8G2_R0, SCREEN_CS, SCREEN_DC, SCREEN_RST);

unsigned long sound_trigger_mark = 0;
bool sound_trigger = false;
bool targeting_mode = true;

#define LED_BLINK_PERIOD 50
#define LED_BLINK_TIME 500

unsigned long led_blink_start = 0;
unsigned long last_led_blink = 0;
bool led_blink_active = false;
bool led_blink = false;

void setup() {
    Serial.begin(115200);
    pinMode(TRIGGER_IN, INPUT);

    pinMode(TRIGGER_OUT, OUTPUT);
    digitalWrite(TRIGGER_OUT, LOW);

    //pinMode(FRONT_LED, OUTPUT);
    //digitalWrite(FRONT_LED, LOW);

    pinMode(BACK_LED, OUTPUT);
    digitalWrite(BACK_LED, LOW);

    screen.begin();
    initBattery();
    initFrontLED();
    initAnimation();
}

void loop() {
    digitalWrite(TRIGGER_OUT, sound_trigger ? HIGH : LOW);
    //digitalWrite(FRONT_LED, (targeting_mode && led_blink) ? LOW : HIGH);
    digitalWrite(BACK_LED, (targeting_mode && led_blink) ? LOW : HIGH);

    readBattery();

    // first, check the trigger state
    if (targeting_mode && digitalRead(TRIGGER_IN) == HIGH) {
        targeting_mode = false;
        beginSnipe();

        // will pass on trigger signal to sound board
        unsigned long curr_t = millis();
        sound_trigger_mark = curr_t;
        sound_trigger = true;

        // start led blinking
        led_blink_start = curr_t;
        last_led_blink = curr_t;
        led_blink_active = true;
        led_blink = true;

        startFrontLEDSequence();
    }

    // cutoff sound trigger
    unsigned long curr_t = millis();
    if (sound_trigger && (curr_t < sound_trigger_mark || curr_t >= sound_trigger_mark + SOUND_TRIGGER_TIME)) {
        sound_trigger = false;
    }

    if (led_blink_active) {
        if (curr_t < led_blink_start || curr_t >= led_blink_start + LED_BLINK_TIME) {
            // cutoff led blink
            led_blink_active = false;
            led_blink = true;
        } else if (curr_t < last_led_blink || curr_t >= last_led_blink + LED_BLINK_PERIOD) {
            // blink led
            led_blink = !led_blink;
            last_led_blink = curr_t;
        }
    }

    updateFrontLED();
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