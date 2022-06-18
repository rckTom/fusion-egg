#pragma once

#include <complex>

void fft(std::complex<float> *input, std::complex<float> *output, uint32_t N);
void rfft(std::complex<float> *input, std::complex<float> *output, uint32_t N);