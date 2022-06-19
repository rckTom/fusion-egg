#include "app.h"
#include "fft.h"
#include "zauberstab.h"

#define N_SAMPLES 256

static std::complex<float> samples[N_SAMPLES];
static std::complex<float> z[N_SAMPLES];
static double vReal[N_SAMPLES];
static double vImag[N_SAMPLES];
static uint32_t sample_counter = 0;
static unsigned long max_dt = 0;
static unsigned long last_sample = 0;

void FFTTestApp::init()
{
}

void FFTTestApp::deinit()
{

}

void FFTTestApp::loop()
{
    if (micros() - last_sample >= 200)
    {
        unsigned long dt = micros() - last_sample;
        last_sample = micros();
        if (dt > max_dt)
        {
            max_dt = dt;
        }

        samples[sample_counter++] = get_sample();

        if (sample_counter == N_SAMPLES)
        {
            unsigned long start = micros();
            FFT<float>::fft(samples, z, N_SAMPLES);
            unsigned long end = micros();

            float max = 0.f;
            for (int i = 0; i < N_SAMPLES / 2; i++)
            {
                float v = std::log10(std::abs(z[i]));
                if (v > max)
                {
                    max = v;
                }
            }

            for (int i = 0; i < N_SAMPLES / 2; i++)
            {
                float v = std::log10(std::abs(z[i]));
                if (v < 2.f)
                {
                    v = 0.f;
                }
                uint8_t led_value = v * 20;
                leds[i].setRGB(led_value, led_value, led_value);
            }

            sample_counter = 0;
            start = micros();
            FastLED.show();
            end = micros();
        }
    }
}