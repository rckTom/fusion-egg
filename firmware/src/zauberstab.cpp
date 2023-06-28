/* 
 * Copyright 2023 Alexander Alber and Thomas Schmid
 * 
 * SPDX-License-Identifier: GPL-2.0
 */

#include "zauberstab.h"
#include "dc_cancelation.h"

DcCancelation<float> dc_blocker{0.95};
CRGB leds[NUM_LEDS];
static int16_t mic_offset = 0;
static bool acc_event = false;
ADXL345_WE myAcc = ADXL345_WE(ADXL345_I2CADDR);

static uint16_t read_mic()
{
    return analogRead(MIC_PIN);
}

void double_tab_int() {
    acc_event = true;
}

bool acc_has_event() {
    return acc_event;
}

int zauberstab_init()
{
    FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812, LED_PIN3, GRB>(leds, NUM_LEDS);

    Wire.begin();
    if (!myAcc.init()){
        Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
        return -1;
    }

    myAcc.setDataRate(ADXL345_DATA_RATE_200);
    myAcc.setRange(ADXL345_RANGE_16G);
    myAcc.setGeneralTapParameters(ADXL345_XY0, 5.0, 50, 100.0);
    myAcc.setAdditionalDoubleTapParameters(false, 250);
    myAcc.setInterrupt(ADXL345_DOUBLE_TAP, INT_PIN_1);
    attachInterrupt(digitalPinToInterrupt(4), double_tab_int, RISING);

    return 0;
}

float get_sample()
{
    float sample = read_mic();
    sample = dc_blocker.update(sample);
    return sample;
}