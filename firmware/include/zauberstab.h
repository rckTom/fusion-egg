/* 
 * Copyright 2023 Alexander Alber and Thomas Schmid
 * 
 * SPDX-License-Identifier: GPL-2.0
 */

#pragma once

#include "Arduino.h"
#include <FastLED.h>
#include <Wire.h>
#include <ADXL345_WE.h>

#define ADXL345_I2CADDR 0x53  // 0x1D if SDO = HIGH

#define LED_PIN1 12
#define LED_PIN2 14
#define LED_PIN3 27
#define NUM_LEDS 48
#define MIC_PIN 15

extern CRGB leds[NUM_LEDS];
extern ADXL345_WE myAcc;

int zauberstab_init();
float get_sample();
void switch_app();
bool acc_has_event();