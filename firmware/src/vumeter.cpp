#include "zauberstab.h"
#include "pt1.h"

unsigned long last_sample_time;
static int sample_counter = 0;
unsigned int top_led_pos = 0; 
float rms_avg = 0;
float vu_filt = 0.0f;
float vu_filt_slow = 0.0f;
float dt;

struct pt1_state vu_pt1_fast;
struct pt1_state vu_pt1_slow;

void setup()
{
    zauberstab_init();
    Serial.begin(115200);
    FastLED.setBrightness(100);

    pt1_init(&vu_pt1_slow, 1, 1.f);
    pt1_init(&vu_pt1_fast, 1, 0.05);
}

void loop() {
    if (micros()-last_sample_time >= 500){
        last_sample_time = micros();
        int32_t sample = get_sample();
        float in = sample*sample;
        sample_counter++;
        rms_avg += (in - rms_avg)/(sample_counter + 1);
    }
    
    EVERY_N_MILLIS(10){

        float vu = 20 * log10f(rms_avg);
        vu_filt = pt1_update(&vu_pt1_fast, vu, 0.01f);
        vu_filt_slow = pt1_update(&vu_pt1_slow, vu_filt, 0.01f);
        //Serial.println(vu);
        int max_led = vu_filt;
        int top_led = vu_filt_slow;

        max_led = max_led > 0xFF ? 0xFF : max_led;

        if (top_led < max_led){
            vu_pt1_slow.y_n1 = vu_filt;
            top_led = max_led;
        }

        fill_solid(leds, NUM_LEDS, CRGB::Black);
        for(int i = 0; i < max_led; i++) {
            int idx = map(i, 0, NUM_LEDS, 0, 0xFF);
            leds[i] = ColorFromPalette(RainbowColors_p, idx);
        }
        
        leds[top_led] = CRGB::White;

        FastLED.show();
        rms_avg = 0.0f;
        sample_counter = 0;
    }
}