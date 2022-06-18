#include "dc_cancelation.h"

void dc_cancelation_init(struct dc_cancelation_state *state, float R){
    state->R = R; 
    state->x_n1 = 0.0f;
    state->y_n1 = 0.0f;
}

void dc_cancelation_set_R(struct dc_cancelation_state *state, float R) {
    state->R = R;
}

float dc_cancelation_update(struct dc_cancelation_state *state, float x) {
    float y = x-state->x_n1 + state->R * state->y_n1;
    state->x_n1 = x;
    state->y_n1 = y;
    return y;
}
