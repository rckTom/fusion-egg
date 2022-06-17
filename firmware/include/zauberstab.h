#pragma once

#include <FastLED.h>

#define LED_PIN 12
#define NUM_LEDS 144
#define MIC_PIN 15

extern CRGB leds[NUM_LEDS];

int zauberstab_init();
int32_t get_sample();