#include "zauberstab.h"
#include "dc_cancelation.h"

DcCancelation<float> dc_blocker{0.95};
CRGB leds[NUM_LEDS];
static int16_t mic_offset = 0;

static uint16_t read_mic() {
    return analogRead(MIC_PIN);
}

int zauberstab_init() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    //FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
    return 0;
}

float get_sample() {
    float sample = read_mic();
    sample = dc_blocker.update(sample);
    return sample;
}