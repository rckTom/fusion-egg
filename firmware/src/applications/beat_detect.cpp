/* 
 * Copyright 2023 Alexander Alber and Thomas Schmid
 * 
 * SPDX-License-Identifier: GPL-2.0
 */

#include <algorithm>

#include "app.h"
#include "biquad.h"
#include "pt1.h"
#include "zauberstab.h"

#undef NUM_LEDS
#define NUM_LEDS 48

#define SAMPLING_FREQUENCY_BP 40 // number of energy chunks per second
#define SAMPLING_FREQUENCY_CONTROL \
    1         // check number of times per second if the current band pass is the best
              // one
#define Q 30. // quality factor of band pass filters
#define PI 3.1415926535897932384626433832795
#define n_BP 40 // number of band pass filters

static const unsigned long sampling_period_bp = 1000000L / SAMPLING_FREQUENCY_BP;
static const unsigned long sampling_period_control = 1000000L / SAMPLING_FREQUENCY_CONTROL;
static float energy = 0;
static unsigned long last_us_bp = 0L;
static unsigned long last_us_control = 0L;

static Biquad<float> bp_filters[n_BP];
static Pt1<float> y_filter[n_BP];
static Pt1<float> pos_filter{1.f, 1.f};

static float yy1[n_BP];
static float yy2[n_BP];
static float yy3[n_BP];
static float yy4[n_BP];
static float yy5[n_BP];
static float yy6[n_BP];
static float y[n_BP];
static float y_fil[n_BP];

static float angle;
static float angle2;

// static double energy_fil = 800.;

static float pos_target = NUM_LEDS / 2;
static float pos_target_filtered = NUM_LEDS / 2;
static long initial_time;

static int active = 15;
static int rounds = 0;
static int n_samples = 0;

static int
get_value(int pos, float pos0)
{
    if (abs(pos0 - pos) > 5)
    {
        return 0;
    }
    else
    {
        return (30 - abs(pos0 - pos) * 6);
    }
}

static void
set_filter()
{
    for (int i = 0; i < n_BP; i++)
    {
        float frequency = 1.75 + i * (2.5 - 1.75) / n_BP;
        float a, a0, a1, a2, b0, b1, b2, w0;
        w0 = 2. * PI * frequency / SAMPLING_FREQUENCY_BP;
        a = sin(w0 / (2. * Q));
        b0 = a;
        b1 = 0.f;
        b2 = -a;
        a0 = 1.f + a;
        a1 = -2.f * cos(w0);
        a2 = 1.f - a;
        bp_filters[i] = Biquad<float>{a0, a1, a2, b0, b1, b2};
        y_filter[i] = Pt1<float>{1.f, 1.f};
    }
}

void BeatDetectApp::init()
{
    set_filter();
    initial_time = micros();
    pos_target = NUM_LEDS / 2;
    pos_target_filtered = NUM_LEDS / 2;
    active = 15;
    rounds = 0;
    n_samples = 0;

    pos_filter.reset();

    for (int i = 0; i<n_BP; i++){
        bp_filters[i].reset();
    }
}

void BeatDetectApp::deinit()
{

}

void BeatDetectApp::loop()
{
    float sample = get_sample();
    
    energy += std::abs(sample) * std::abs(sample);
    n_samples++;

    if (micros() - last_us_bp > sampling_period_bp)
    {

        n_samples = 0;
        last_us_bp = micros();
        // energy_fil += (energy - energy_fil) * 0.01;

        for (int i = 0; i < n_BP; i++)
        {
            y[i] = bp_filters[i].update(energy);
            yy6[i] = yy5[i];
            yy5[i] = yy4[i];
            yy4[i] = yy3[i];
            yy3[i] = yy2[i];
            yy2[i] = yy1[i];
            yy1[i] = y[i];
            y_fil[i] = y_filter[i].update(std::abs(y[i]),
                                       0.005f); // linie der scheitelpunkte
                                                // y_fil[i] += (abs(y[i]) - y_fil[i]) * 0.005; //linie der
                                                // scheitelpunkte
        
        }


        float delays = constrain(SAMPLING_FREQUENCY_BP * 0.25 / (1.75 + active * (2.5 - 1.75) / n_BP),
                                 4., 6.);

        float delayed = 0;
        if (delays > 5)
        {
            delayed = yy5[active] * (1 - delays + 5) + yy6[active] * (delays - 5);
        }
        else
        {
            delayed = yy4[active] * (1 - delays + 4) + yy5[active] * (delays - 4);
        }

        angle = atan2(delayed, y[active]);

        

        if (PI < abs(angle - angle2) && abs(angle - angle2) < 3 * PI)
        {
            angle2 = angle + 2 * PI;
        }
        else
        {
            angle2 = angle;
        }

        pos_target = map(angle2, -PI, 3 * PI, -0.3 * NUM_LEDS, NUM_LEDS * 1.5);
        //pos_target = NUM_LEDS * (sin(angle2)+1)/2;



        if (pos_target > pos_target_filtered)
        {
            pos_target_filtered = pos_filter.update(pos_target, 0.35f);
        }
        else
        {
            pos_filter.y_n1 = pos_target;
            pos_target_filtered = pos_target;
        }


        energy = 0;

        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i].g = get_value(i, pos_target_filtered);
            leds[i].r = get_value(i, pos_target_filtered + 2);
            leds[i].b = get_value(i, pos_target_filtered - 2);

            //leds[i].g = get_value(i, pos_target_filtered);
            //leds[i].g = get_value(i, pos_target_filtered + 2);
            //leds[i].b = get_value(i, pos_target_filtered - 2);

        }
        FastLED.show();
    }

    if (micros() - last_us_control > sampling_period_control)
    {
        last_us_control = micros();
        int argmax = -1;
        float valuemax = 0;
        for (int i = 0; i < n_BP; i++)
        {
            if (y_fil[i] > valuemax)
            {
                valuemax = y_fil[i];
                argmax = i;
            }
        }

        if (argmax != active)
        {
            rounds ++;
        }

        if (rounds > 5)
        {
            active = argmax;
            rounds = 0;
        }

    }
}
