#include "zauberstab.h"

#define SAMPLING_FREQUENCY_BP 40     // number of energy chunks per second
#define SAMPLING_FREQUENCY_CONTROL 1 // check number of times per second if the current band pass is the best one
#define Q 20.                        // quality factor of band pass filters
#define PI 3.1415926535897932384626433832795
#define n_BP 30 //number of band pass filters

static unsigned long sampling_period_bp = 1000000L / SAMPLING_FREQUENCY_BP;
static unsigned long sampling_period_control = 1000000L / SAMPLING_FREQUENCY_CONTROL;
static double energy = 0;
static unsigned long last_us_bp = 0L;
static unsigned long last_us_control = 0L;

static float a0[n_BP];
static float a1[n_BP];
static float a2[n_BP];
static float b0[n_BP];
//static float b1[n_BP];
static float b2[n_BP];

static float a[n_BP];
static float w0[n_BP];

static float yy1[n_BP];
static float yy2[n_BP];
static float yy3[n_BP];
static float yy4[n_BP];
static float yy5[n_BP];
static float yy6[n_BP];

static float u1[n_BP];
static float u2[n_BP];
static float y[n_BP];
static float y_fil[n_BP];

static float angle;
static float angle2;

static double energy_fil = 800.;

static float pos_target = NUM_LEDS / 2;
static float pos_target_filtered = NUM_LEDS / 2;

static float microphone_offset = 675;
static long initial_time;

static int active = 15;
static int candidate = 15;
static int rounds = 0;

static int get_value(int pos, float pos0)
{
    if (abs(pos0 - pos) > 20)
    {
        return 0;
    }
    else
    {
        return (40 - abs(pos0 - pos) * 2);
    }
}

static void set_filter()
{
    for (int i = 0; i < n_BP; i++)
    {
        float frequency = 1.75 + i * (2.4 - 1.75) / n_BP;
        w0[i] = 2. * PI * frequency / SAMPLING_FREQUENCY_BP;
        a[i] = sin(w0[i] / (2. * Q));
        b0[i] = a[i];
        //b1[i] = 0;
        b2[i] = -a[i];
        a0[i] = 1. + a[i];
        a1[i] = -2. * cos(w0[i]);
        a2[i] = 1. - a[i];
    }
}

void setup()
{
    zauberstab_init();
    Serial.begin(115200);
    set_filter();
    initial_time = micros();
}
void loop()
{

    int sample = int(analogRead(MIC_PIN) - microphone_offset);
    energy += abs(sample) * abs(sample);

    if (micros() - last_us_bp > sampling_period_bp)
    {

        Serial.println(sample);

        microphone_offset += (analogRead(MIC_PIN) - microphone_offset) * 0.001;

        //Serial.println(microphone_offset);

        last_us_bp += sampling_period_bp;
        energy_fil += (energy - energy_fil) * 0.01;
        //Serial.println(energy);
        for (int i = 0; i < n_BP; i++)
        {
            y[i] = (b0[i] / a0[i]) * energy + 0. + (b2[i] / a0[i]) * u2[i] - (a1[i] / a0[i]) * yy1[i] - (a2[i] / a0[i]) * yy2[i];
            u2[i] = u1[i];
            u1[i] = energy;
            yy6[i] = yy5[i];
            yy5[i] = yy4[i];
            yy4[i] = yy3[i];
            yy3[i] = yy2[i];
            yy2[i] = yy1[i];
            yy1[i] = y[i];
            y_fil[i] += (abs(y[i]) - y_fil[i]) * 0.005; //linie der scheitelpunkte
        }

        float delays = constrain(SAMPLING_FREQUENCY_BP * 0.25 / (1.75 + active * (2.4 - 1.75) / n_BP), 4., 6.);

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

        if (pos_target > pos_target_filtered)
        {
            pos_target_filtered += (pos_target - pos_target_filtered) * 0.35;
        }
        else
        {
            pos_target_filtered = pos_target;
        }

        //       Serial.print(y_fil[active]);
        //       Serial.print(",");
        //       Serial.println(y[active]);

        energy = 0;

        for (int i = 0; i < NUM_LEDS; i++)
        {
            int brightness = get_value(i, pos_target_filtered);
            if (brightness >= 1) {
                brightness = 10;
            }
            //leds[i].setRGB(brightness, brightness, brightness);
            leds[i].setHSV(160, (rounds == 6) ? 0xFF : 0, brightness);
        }
        FastLED.show();
    }

    if (micros() - last_us_control > sampling_period_control)
    {
        last_us_control += sampling_period_control;
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

        if (argmax > -1)
        {
            if (argmax == candidate)
            {
                rounds++;
            }
            else
            {
                rounds = 0;
                candidate = argmax;
            }
            if (rounds > 6)
            {
                rounds = 0;
                active = candidate;
            }
        }
    }
}
