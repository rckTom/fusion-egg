#include <algorithm>

#include "app.h"
#include "biquad.h"
#include "pt1.h"
#include "zauberstab.h"

#undef NUM_LEDS
#define NUM_LEDS 46

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
static float angle_pre = 0.1;
static long initial_time;

static int active = 15;
static int rounds = 0;
static int n_samples = 0;

static int beat_count = 1;
static int quarter = 0;

static int quarter_color_h1 = 30;
static int quarter_color_s = 255;
static int quarter_color_v = 50;


//modus 0: farbverlauf, farben wechseln alle 4 beats
//modus 1: konstante farbe, wechselt bei jedem beat
//modus 2: weiß, zählt binärzahlen hoch
static int modus = 0;  
static int modus_count = 0;



static int get_value(int pos, int quarter, int beat_count, char component)
{


    if (modus == 3)
    {
        if (component == 'h'){return 255;}
        if (component == 's'){return 0;}
        if (component == 'v')
        {
           if ((quarter+pos)%2==0)
           {return 50;}
           else
           {return 0;}
        }
    }


    else if (modus == 2)
    {
        if (component == 'h'){return 255;}
        if (component == 's'){return 0;}
        if (component == 'v')
        {
            int v = 50;
            int s1=0, s2=0, s3=0, s4=0;
            switch (beat_count)
            {
                case 0:
                    s1 = 0, s2 = 0, s3 = 0, s4 = 0;
                    break;
                case 1:
                    s1 = 0, s2 = 0, s3 = 0, s4 = 1;
                    break;
                case 2:
                    s1 = 0, s2 = 0, s3 = 1, s4 = 0;
                    break;
                case 3:
                    s1 = 0, s2 = 0, s3 = 1, s4 = 1;
                    break;
                case 4:
                    s1 = 0, s2 = 1, s3 = 0, s4 = 0;
                    break;
                case 5:
                    s1 = 0, s2 = 1, s3 = 0, s4 = 1;    
                    break;     
                case 6:
                    s1 = 0, s2 = 1, s3 = 1, s4 = 0;
                    break;
                case 7:
                    s1 = 0, s2 = 1, s3 = 1, s4 = 1;
                    break;
                case 8:
                    s1 = 1, s2 = 0, s3 = 0, s4 = 0;
                    break;
                case 9:
                    s1 = 1, s2 = 0, s3 = 0, s4 = 1;
                    break;
                case 10:
                    s1 = 1, s2 = 0, s3 = 1, s4 = 0;
                    break;
                case 11:
                    s1 = 1, s2 = 0, s3 = 1, s4 = 1;
                    break;
                case 12:
                    s1 = 1, s2 = 1, s3 = 0, s4 = 0;
                    break;
                case 13:
                    s1 = 1, s2 = 1, s3 = 0, s4 = 1;
                    break;
                case 14:
                    s1 = 1, s2 = 1, s3 = 1, s4 = 0;
                    break;
                case 15:
                    s1 = 1, s2 = 1, s3 = 1, s4 = 1;  
                    break;                    
            }

            if (pos < NUM_LEDS/4) {return s1*v;}
            else if (pos < NUM_LEDS/2) {return s2*v;}
            else if (pos < 3*NUM_LEDS/4) {return s3*v;}
            else {return s4*v;}
        }
    }


    else if (pos < NUM_LEDS*quarter/3)
    {
        if (component == 'h')
        {
            if (modus == 0)
            {
                
                int color = quarter_color_h1 + 50*pos/NUM_LEDS;
                return color%255;
            }
            return quarter_color_h1;

        }
        if (component == 's')
        {
            return quarter_color_s;
        }
        if (component == 'v')
        {
            return quarter_color_v;
        }
    }
    return 0;
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

void QuarterApp::init()
{
    set_filter();
    initial_time = micros();
    active = 15;
    rounds = 0;
    n_samples = 0;

    pos_filter.reset();

    for (int i = 0; i<n_BP; i++){
        bp_filters[i].reset();
    }
}

void QuarterApp::deinit()
{

}

void QuarterApp::loop()
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





        //every beat
        if (angle > 0 and angle_pre <= 0)
        {
            quarter = beat_count%4;


            //mode 0 logic if active
            if (modus == 0 & quarter == 0)
            {
                quarter_color_h1 += 10;
                quarter_color_h1 = quarter_color_h1%255;
            }

            //mode 1 logic if active
            else if (modus == 1)
            {
                quarter_color_h1 += 15;
                quarter_color_h1 = quarter_color_h1%255;
                quarter_color_s = 255;
            }



            //every 4 beats
            else if (quarter == 0 & modus == 2)
            {
                quarter_color_s = 0;
            }
            else
            {
                quarter_color_s = 255;
            }

            // state machine
            beat_count++;
            if (beat_count > 15)
            {
                beat_count = 0;
                modus_count++;

                if (modus_count > 7)
                {
                    modus_count = 0;
                    modus++;
                    if (modus > 3)
                    {
                        modus = 0;
                    }
                }



            }


        }

        for (int i = 0; i < NUM_LEDS; i++)
        {

            int h = get_value(i, quarter, beat_count, 'h');
            int s = get_value(i, quarter, beat_count, 's');
            int v = get_value(i, quarter, beat_count, 'v');

            leds[i].setHSV(h, s, v);

        }
        FastLED.show();
        
        angle_pre = angle;
        energy = 0;
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
