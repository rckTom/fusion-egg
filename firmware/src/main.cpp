#include "app.h"
#include "driver/adc.h"
#include "zauberstab.h"
#include <vector>

struct BeatDetectApp beat_detect_app
{
};
struct VuMeterApp vu_meter_app
{
};
struct FFTTestApp fft_test_app
{
};
struct FackelApp fackel_app
{
};


std::vector<std::reference_wrapper<App>> apps = {
    std::ref<App>(beat_detect_app),
    std::ref<App>(fackel_app)
    };

unsigned int current_app = 1;
unsigned int next_app;

void setup()
{
    next_app = current_app;
    zauberstab_init();
    //Serial.begin(115200);

    apps[current_app].get().init();
}

void loop()
{
/*     EVERY_N_SECONDS(30)
    {
        next_app++;
        next_app = next_app % apps.size();
    } */

    if (next_app != current_app)
    {
        apps[current_app].get().deinit();
        apps[next_app].get().init();
        current_app = next_app;
        fadeToBlackBy(leds, NUM_LEDS, 0xFF);
    }

    apps[current_app].get().loop();
}