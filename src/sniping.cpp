#include "nerdsniper.h"

#define MAX_LINES 200
#define LINE_BUFFER_SIZE 30
#define LINE_SPACING 2

#define SCROLL_SLOWDOWN 2
#define SCROLL_START 1000
#define SNIPE_TIME 5000

const char* snipe_00 =
    "On an island of deaf mute logicians, 100 people have blue eyes, 100 people have brown eyes, and 1 guru has green eyes. "
    "Everyone sees everyone else's eyes, but no one knows their own eye color. "
    "Each night a ferry comes, and every logician who knows their own eye color leaves. "
    "On day 0, the guru speaks only one time. "
    "She says, \"I see a person with blue eyes\". "
    "Who leaves the island and when?";
const char* snipe_01 =
    "You have two eggs and access to a 100 story building. "
    "The eggs are identical and will break when dropped from floor N up. "
    "How do you determine N with the fewest drops? ";
const char* snipe_02 =
    "100 prisoners with life sentences are held in solitary confinement. "
    "Each day the warden brings one chosen at random to a room with a single light bulb. "
    "The prisoner can choose to do nothing, flip the switch, or call balderdash. "
    "If they call balderdash after all prisoners have visited the room once, all are freed. "
    "If they call balderdash early, all are shot. They have one chance to plan. "
    "What plan do they make?";
#define N_SNIPES 3
const char *const snipes[] = {snipe_00, snipe_01, snipe_02};

const char* curr_snipe = nullptr;

size_t n_lines = 0;
const char* line_positions[MAX_LINES];
size_t line_lengths[MAX_LINES];
u8g2_int_t line_height = 0;

u8g2_int_t scroll = 0;
uint8_t scroll_phase = 0;
bool scroll_finished = false;
bool scroll_at_bottom = false;
unsigned long snipe_mark = 0;

#define FONT u8g2_font_7x13B_tf

void beginSnipe() {
    curr_snipe = snipes[random(0, N_SNIPES)];
    const size_t buffer_len = strlen(curr_snipe);

    // calculate word lengths
    screen.setFont(FONT);
    line_height = screen.getAscent() - screen.getDescent() + LINE_SPACING;
    n_lines = 1;
    line_positions[0] = curr_snipe;
    char line_buffer[LINE_BUFFER_SIZE];
    size_t line_pos = 0;
    for (size_t i = 0; i < buffer_len; i++) {
        line_buffer[line_pos] = curr_snipe[i];
        line_buffer[line_pos + 1] = 0;
        line_pos++;
        if (screen.getStrWidth(line_buffer) > SCREEN_W) {
            if (curr_snipe[i] != ' ') {
                // first check if there are any spaces on the line
                bool has_space = false;
                for (int j = line_pos; j >= 0; j--) {
                    if (line_buffer[j] == ' ') { has_space = true; break; }
                }
                
                // if so, rewind the break to there
                if (has_space) {
                    while (line_buffer[line_pos] != ' ') {
                        line_pos--; i--;
                    }
                }
            }
            line_lengths[n_lines - 1] = line_pos;
            line_pos = 0;
            while (i + 1 < buffer_len && curr_snipe[i + 1] == ' ') {
                i++;
            }
            if (i + 1 < buffer_len) {
                line_positions[n_lines] = &(curr_snipe[i + 1]);
                n_lines++;
            }
        }
    }
    line_lengths[n_lines - 1] = line_pos;
    scroll = 0;
    scroll_finished = false;
    scroll_at_bottom = false;

    // Print snipe to serial for debugging
    for (size_t i_line = 0; i_line < n_lines; i_line++) {
        for (size_t i = 0; i < line_lengths[i_line]; i++) {
            Serial.print(line_positions[i_line][i]);
        }
        Serial.println();
    }

    snipe_mark = millis();
}

void updateSnipe() {
    const unsigned long curr_t = millis();
    if (n_lines * line_height - scroll > SCREEN_H) {
        if (curr_t > snipe_mark && curr_t <= snipe_mark + SCROLL_START) {
            return;
        }

        // still scrolling
        scroll_phase++;
        if (scroll_phase == SCROLL_SLOWDOWN) {
            scroll_phase = 0;
            scroll++;
        }
    } else if (!scroll_at_bottom) {
        // first frame after scroll is finished, start the clock
        scroll_at_bottom = true;
        snipe_mark = millis();
    } else {
        if (curr_t < snipe_mark || curr_t >= snipe_mark + SNIPE_TIME) {
            scroll_finished = true;
            if (digitalRead(TRIGGER_IN) == HIGH) {
                // if the trigger is still depressed, restart the scroll
                scroll_at_bottom = false;
                scroll = 0;
                snipe_mark = millis();
            } else {
                // reset when snipe is finished
                targeting_mode = true;
            }
        }
    }
}

void drawSnipe() {
    screen.setFont(FONT);

    u8g2_int_t h = screen.getAscent() - scroll;
    const u8g2_int_t max_h = SCREEN_H + screen.getAscent();
    char line_buffer[LINE_BUFFER_SIZE];
    for (size_t i_line = 0; i_line < n_lines; i_line++) {
        if (h > 0 && h <= max_h) {
            memset(line_buffer, 0, LINE_BUFFER_SIZE * sizeof(char));
            memcpy(line_buffer, line_positions[i_line], line_lengths[i_line] * sizeof(char));
            screen.drawStr(0, h, line_buffer);
        }
        h += line_height;
    }
}