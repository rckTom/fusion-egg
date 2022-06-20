#include <algorithm>

#include "app.h"
#include "biquad.h"
#include "pt1.h"
#include "zauberstab.h"
#include "fft.h"

#undef NUM_LEDS
#define NUM_LEDS 45

#define SAMPLING_FREQUENCY_BP 40 // number of energy chunks per second
#define Q 20. // quality factor of band pass filters
#define PI 3.1415926535897932384626433832795

#define N_SAMPLES 512

static const unsigned long sampling_period_bp = 1000000L / SAMPLING_FREQUENCY_BP;
static float energy = 0;
static unsigned long last_us_bp = 0L;
static unsigned long last_us_control = 0L;

static Biquad<float> bp_filter;
static Pt1<float> y_filter{1.f, 1.f};
static Pt1<float> pos_filter{1.f, 1.f};

static float yy1;
static float yy2;
static float yy3;
static float yy4;
static float yy5;
static float yy6;
static float y;
static float y_fil;
static float angle;
static float angle2;
static float pos_target = NUM_LEDS / 2;
static float pos_target_filtered = NUM_LEDS / 2;
static long initial_time;

static std::complex<float> samples[N_SAMPLES];
static std::complex<float> z[N_SAMPLES];
static uint32_t sample_counter = 0;
static unsigned long max_dt = 0;
static unsigned long last_sample = 0;



static int
get_value(int pos, float pos0)
{
    if (abs(pos0 - pos) > 5)
    {
        return 0;
    }
    else
    {
        return (40 - abs(pos0 - pos) * 8);
    }
}

static void
set_filter(float frequency)
{
    float a, a0, a1, a2, b0, b1, b2, w0;
    w0 = 2. * PI * frequency / SAMPLING_FREQUENCY_BP;
    a = sin(w0 / (2. * Q));
    b0 = a;
    b1 = 0.f;
    b2 = -a;
    a0 = 1.f + a;
    a1 = -2.f * cos(w0);
    a2 = 1.f - a;
    bp_filter = Biquad<float>{a0, a1, a2, b0, b1, b2};
}

void FftDetectApp::init()
{
    set_filter(2.0f);
    initial_time = micros();
    pos_target = NUM_LEDS / 2;
    pos_target_filtered = NUM_LEDS / 2;
    pos_filter.reset();
    bp_filter.reset();
}

void FftDetectApp::deinit()
{

}

void FftDetectApp::loop()
{
    float sample = get_sample();
    energy += std::abs(sample) * std::abs(sample);

    if (micros() - last_us_bp > sampling_period_bp)
    {
        samples[sample_counter++] = energy;

        last_us_bp = micros();
        y = bp_filter.update(energy);
        yy6 = yy5;
        yy5 = yy4;
        yy4 = yy3;
        yy3 = yy2;
        yy2 = yy1;
        yy1 = y;
        y_fil = y_filter.update(std::abs(y), 0.005f); 


        

        float delayed = yy5;
        angle = atan2(delayed, y);

        if (PI < abs(angle - angle2) && abs(angle - angle2) < 3 * PI)
        {
            angle2 = angle + 2 * PI;
        }
        else
        {
            angle2 = angle;
        }

        pos_target = map(angle2, -PI, 3 * PI, -0.3 * NUM_LEDS, NUM_LEDS * 1.5);

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

            // leds[i].setRGB(brightness_red, brightness_green, brightness_blue);
            // leds[i].setHSV(160, (rounds == 6) ? 0xFF : 0, brightness);
        }
        FastLED.show();
    }





    if (sample_counter == N_SAMPLES)
        {


            float samplesum = 0.f;
            for (int i = 0; i < N_SAMPLES;i++)
            {
                samplesum = samplesum + std::abs(samples[i]);
            }

            float sampleavg = samplesum/N_SAMPLES;

            for (int i = 0; i < N_SAMPLES;i++)
            {
                samples[i] =  samples[i]  - sampleavg;
            }

            FFT<float>::fft(samples, z, N_SAMPLES);
            
            float max = 0.f;
            int pos = -1;
            for (int i = 20; i < 30; i++)
            {
                float v = std::abs(z[i]);

                if (v > max)
                {
                    max = v;
                    pos = i;
                }

                Serial.println(v);
            }

            float frequency = 40.f/512.f*pos;
            set_filter(frequency);
            sample_counter = 0;
        }
    
}
