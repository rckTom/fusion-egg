#include "pt1.h"

void pt1_init(struct pt1_state *state, float K, float T)
{
    state->K = K;
    state->T = T;
    state->y_n1 = 0.0f;
}

float pt1_update(struct pt1_state *state, float u, float dt)
{
    float y = state->y_n1 + (state->K * u - state->y_n1) * dt / state->T;
    state->y_n1 = y;
    return y;
}