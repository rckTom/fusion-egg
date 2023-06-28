#include "app.h"
#include "driver/adc.h"
#include "zauberstab.h"
#include <vector>

struct BeatDetectApp beat_detect_app
{
};

struct QuarterApp quarter_app
{
};

struct FackelApp fackel_app
{
};



std::vector<std::reference_wrapper<App>> apps = {
    std::ref<App>(beat_detect_app),
    std::ref<App>(fackel_app),
    std::ref<App>(quarter_app),
    };

static unsigned int current_app = 0;
static unsigned int next_app;
static bool init_successfull = false;
static bool sleep_active=false;

void switch_app() {
    if (!sleep_active) {
        next_app = current_app + 1;
    } else {
        sleep_active = false;
        next_app = 0;
    }

    if (next_app >= apps.size()) {
        next_app = 0;
        // Turn off leds before going to sleep
        fadeToBlackBy(leds, NUM_LEDS, 0xFF);
        FastLED.show();

        //configure wakeup source
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
        
        //bedtime
        sleep_active = true;
        esp_deep_sleep_start();
    }

    next_app = next_app % apps.size();
}

void setup()
{
    next_app = current_app;
    if (zauberstab_init() != 0) {
        return;
    }
    Serial.begin(115200);

    init_successfull = true;
    apps[current_app].get().init();
}

void loop()
{
    if (!init_successfull) {
        return;
    }

    if (acc_has_event()) {
        String axes = myAcc.getActTapStatusAsString();
        byte intSource = myAcc.readAndClearInterrupts();

        if (myAcc.checkInterrupt(intSource, ADXL345_DOUBLE_TAP)) {
            switch_app();
        }
    }

    if (next_app != current_app)
    {
        apps[current_app].get().deinit();
        apps[next_app].get().init();
        current_app = next_app;
        fadeToBlackBy(leds, NUM_LEDS, 0xFF);
    }

    apps[current_app].get().loop();
}