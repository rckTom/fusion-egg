#pragma once

struct App {
    void virtual init() = 0;
    void virtual deinit() = 0;
    void virtual loop() = 0;
};

struct BeatDetectApp : public App {
    void init();
    void deinit();
    void loop();
};

struct VuMeterApp: public App {
    void init();
    void deinit();
    void loop();
};

struct FFTTestApp: public App {
    void init();
    void deinit();
    void loop();
};

struct FackelApp: public App {
    void init();
    void deinit();
    void loop();
};