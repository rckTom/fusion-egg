#pragma once

struct pt1_state {
    float y_n1;
    float K;
    float T;
};

void pt1_init(struct pt1_state *state, float K, float T);
float pt1_update(struct pt1_state *state, float u, float dt);