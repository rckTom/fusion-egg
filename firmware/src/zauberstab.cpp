#include "zauberstab.h"
#include "dc_cancelation.h"

struct dc_cancelation_state dc_blocker;
CRGB leds[NUM_LEDS];
static int16_t mic_offset = 0;

static uint16_t read_mic() {
    return analogRead(MIC_PIN);
}

int zauberstab_init() {
    FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
    dc_cancelation_init(&dc_blocker, 0.95);
    return 0;
}

float get_sample() {
    int32_t raw_sample = read_mic();
    return dc_cancelation_update(&dc_blocker, (float) raw_sample);
}