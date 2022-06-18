#pragma once

struct dc_cancelation_state {
    float x_n1;
    float y_n1;
    float R; 
};

void dc_cancelation_init(struct dc_cancelation_state *state, float R);
void dc_cancelation_set_R(struct dc_cancelation_state *state, float R);
float dc_cancelation_update(struct dc_cancelation_state *state, float x);