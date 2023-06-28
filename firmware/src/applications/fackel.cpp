/* 
 * Copyright 2023 Alexander Alber and Thomas Schmid
 * 
 * SPDX-License-Identifier: GPL-2.0
 */

#include "app.h"
#include "pt1.h"
#include "zauberstab.h"
#include <cstdlib>
#include <string>

#undef NUM_LEDS
#define NUM_LEDS 48

static unsigned long last_sample_time;
static unsigned long sample_counter;
static float rms_avg;

static uint8_t energy[NUM_LEDS + 2];
static uint8_t spark_energy[NUM_LEDS + 2];
char string[128];

Pt1<float> energy_pt1{1.f, 3.f};
Pt1<float> rms_pet1{1.f, 0.05f};
CRGB palette[128];

static void hsl_to_rgb(uint32_t hue, uint32_t sat, uint32_t lum, uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint32_t v;

    v = (lum < 128) ? (lum * (256 + sat)) >> 8 : (((lum + sat) << 8) - lum * sat) >> 8;

    if (v <= 0)
    {
        *r = *g = *b = 0;
    }
    else
    {
        int32_t m;
        int32_t sextant;
        int32_t fract, vsf, mid1, mid2;

        m = lum + lum - v;
        hue *= 6;
        sextant = hue >> 8;
        fract = hue - (sextant << 8);
        vsf = v * fract * (v - m) / v >> 8;
        mid1 = m + vsf;
        mid2 = v - vsf;
        switch (sextant)
        {
        case 0:
            *r = v;
            *g = mid1;
            *b = m;
            break;
        case 1:
            *r = mid2;
            *g = v;
            *b = m;
            break;
        case 2:
            *r = m;
            *g = v;
            *b = mid1;
            break;
        case 3:
            *r = m;
            *g = mid2;
            *b = v;
            break;
        case 4:
            *r = mid1;
            *g = m;
            *b = v;
            break;
        case 5:
            *r = v;
            *g = m;
            *b = mid2;
            break;
        }
    }
}

static void update_energy(uint8_t *energy, size_t s)
{
    for (int i = s; i >= 2; i--)
    {
        energy[i] = (uint8_t)((float)(energy[i - 1] + energy[i - 2]) * 0.45f);
    }
}

void FackelApp::init()
{
    memset(&energy, 0, NUM_LEDS);
    memset(&spark_energy, 0, NUM_LEDS);

    // generate palette
    for (int i = 0; i < 128; i++)
    {
        uint8_t r, g, b;
        hsl_to_rgb(i / 5, 255, i * 2 > 128 ? 128 : i * 2, &r, &g, &b);
        //hsl_to_rgb(i / 5 + 180, 255, i * 2 > 128 ? 128 : i * 2, &r, &g, &b);
        g = g == 1 ? 0 : g;
        b = b == 1 ? 0 : b;
        palette[i].r = r;
        palette[i].g = g;
        palette[i].b = b;
    }
}

void FackelApp::deinit()
{
}

void FackelApp::loop()
{
    if (micros() - last_sample_time >= 500)
    {
        last_sample_time = micros();
        int32_t sample = get_sample();
        float in = sample * sample;
        sample_counter++;
        rms_avg += (in - rms_avg) / (sample_counter + 1);
    }

    EVERY_N_MILLISECONDS(10)
    {
        float rms = rms_pet1.update(rms_avg, 0.01f);
        float e_f = energy_pt1.update(rms_avg, 0.01f);
        //sprintf(string, "/*%f,%f*/", 1.3 * e_f, rms);
        //Serial.println(string);

        Serial.print(rms_avg);
        Serial.print(",");
        Serial.println(e_f);

        if (rms > 1.15 * e_f)
        {
            energy[0] = 128;
            energy[1] = 128;
        }
        else
        {
            energy[0] = 0;
            energy[1] = 10;
        }

        rms_avg = 0.0f;
        sample_counter = 0;
    }

    EVERY_N_MILLISECONDS(45)
    {
        update_energy(energy, NUM_LEDS+2);

        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[NUM_LEDS-i-1] = palette[energy[i+2]];
            
        }
        FastLED.show();
    }
}