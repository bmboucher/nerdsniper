#ifndef __NERDSNIPER_H__
#define __NERDSNIPER_H__

#include <Arduino.h>
#include <U8g2lib.h>

// defined in main.cpp
extern U8G2_SSD1309_128X64_NONAME0_1_4W_HW_SPI screen;
extern bool targeting_mode;

// defined in animate.cpp
extern u8g2_int_t range_level;
extern u8g2_int_t nerd_level;
void initAnimation();
void updateAnimation();

// defined in targeting.cpp
void drawTargetingScreen();

// defined in sniping.cpp
void beginSnipe();
void updateSnipe();
void drawSnipe();

// pinout
#define SCREEN_CS 17
#define SCREEN_DC 20
#define SCREEN_RST 21
#define TRIGGER 1 // TODO: real pin

// screen size
#define SCREEN_W 128
#define SCREEN_H 64
#define C_X (SCREEN_W/2)
#define C_Y (SCREEN_H/2)

// animation speed
#define FRAME_RATE 40
#define FRAME_DELAY (1000/FRAME_RATE)

// crosshair dimensions
#define R (C_Y-6)          // outer circle radius
#define CH_SPACE 3         // min space between circle and axis
#define N_SEGS   4         // number of segments on each axis
#define SEG_L ((R-CH_SPACE)/N_SEGS) // length of each segment
#define RL (SEG_L*N_SEGS)  // adjusted length of axes
#define TICK_MAJ 4         // outer crosshair tick
#define TICK_MIN 2         // small crosshair ticks

// battery icon location
#define BATTERY_X 2
#define BATTERY_Y 5

// range meter dimensions
#define RM_H (SCREEN_H-10) // height of line
#define RM_MX 2            // margin from left edge
#define RM_L 32            // length of meter
#define RM_TICK 3          // size of tickmark
#define RM_LOOSE 10        // the higher this value, the more the meter drifts from center
#define RM_TEXT_HEIGHT (RM_H-RM_TICK-2) // bottom of label

// nerd meter dimensions
#define NM_H (SCREEN_H-8)
#define NM_T ((SCREEN_H-NM_H)/2)
#define NM_W 5
#define NM_B (NM_T+NM_H)
#define NM_L (C_X+R+6)
#define NM_R (NM_L+NM_W)
#define NM_MAX_T (NM_T+10)
#define NM_MED_T (NM_T+22)
#define NM_MIN_VAL 20
#define NM_MARKER_W 12
#define NM_MARKER_H 4

#endif // __NERDSNIPER_H__