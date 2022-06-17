#include "zauberstab.h"

CRGB leds[NUM_LEDS];
static int16_t mic_offset = 0;

static uint16_t read_mic() {
    return analogRead(MIC_PIN);
}

static uint16_t get_mic_offset() {
    float average = 0.f;
    uint32_t num_samples = 0;

    for(num_samples = 0; num_samples < 10000; num_samples++) {
        uint16_t sample = read_mic();
        average += ((float) sample - average) / (num_samples + 1);
    }
    
    return (uint16_t)average;
}

int zauberstab_init() {
    FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
    //FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
    mic_offset = get_mic_offset();
    return 0;
}

int32_t get_sample() {
    return read_mic() - mic_offset;
}