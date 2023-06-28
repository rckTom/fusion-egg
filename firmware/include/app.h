/* 
 * Copyright 2023 Alexander Alber and Thomas Schmid
 * 
 * SPDX-License-Identifier: GPL-2.0
 */

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


struct QuarterApp : public App {
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

/**
 * struct ImageDisplayApp: public App {
    void init();
    void deinit();
    void loop();
};
**/