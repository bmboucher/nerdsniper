# Nerd Sniper

Firmware for a "nerd sniper": a foam-dart toy gun fitted with an OLED scope that,
when you pull the trigger, scrolls a classic logic puzzle up the screen — so your
"target" gets nerd-sniped (à la [XKCD #356](https://xkcd.com/356/)) instead of
shot. It's a prop/gag, not a real firearm.

## What it does

- **Targeting mode (idle):** The 128x64 OLED shows an animated sniper-scope HUD —
  crosshair, a drifting "RANGE" meter, a battery gauge, and a wandering "nerd"
  meter. A front NeoPixel and rear LED stay dark.
- **On trigger pull:** Pulses an external sound board, blinks the LEDs through a
  "shot" sequence (white flash, then holds red), and scrolls a randomly selected
  logic riddle (blue-eyed islanders, the two-egg drop problem, the 100 prisoners
  and a lightbulb) up the scope. Hold the trigger to replay.

## Hardware

- Raspberry Pi Pico (RP2040)
- SSD1309 128x64 SPI OLED
- WS2812 NeoPixel (front), plain LED (rear)
- Trigger input + trigger-passthrough output to an external sound board
- LiPo powered; battery voltage read via the VSYS ADC through a 1/3 divider

See pin assignments in `include/nerdsniper.h`.

## Code layout

| File              | Responsibility                                      |
|-------------------|-----------------------------------------------------|
| `main.cpp`        | Setup, main loop, trigger/LED/sound state machine   |
| `targeting.cpp`   | Scope HUD: crosshair, range/nerd/battery meters     |
| `animate.cpp`     | Frame timing and meter animation                    |
| `sniping.cpp`     | Riddle text: word-wrap, scroll engine, the puzzles  |
| `battery.cpp`     | VSYS ADC sampling with exponential smoothing        |
| `front_led.cpp`   | NeoPixel "shot" sequence                            |

## Build

Built with [PlatformIO](https://platformio.org/). Dependencies (U8g2, Adafruit
NeoPixel) are declared in `platformio.ini`.

    pio run            # build
    pio run -t upload  # flash the Pico

## License

MIT — see [LICENSE](LICENSE).
