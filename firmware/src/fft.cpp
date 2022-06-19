#include "fft.h"
#include "math.h"

unsigned int bitReverse(unsigned int x, int log2n)
{
    int n = 0;
    for (int i = 0; i < log2n; i++)
    {
        n <<= 1;
        n |= (x & 1);
        x >>= 1;
    }
    return n;
}

void fft(std::complex<float> *samples, std::complex<float> *output, uint32_t N)
{
    uint8_t log2n = (uint8_t)std::log2(N) + 0.5f;
    std::complex<float> I(0.0, 1.0);
    if (N == 1)
    {
        output[0] = samples[0];
        return;
    }

    // shuffle array
    for (int i = 0; i < N; i++)
    {

        output[i] = samples[bitReverse(i, log2n)];
    }
    for (int s = 1; s <= log2n; s++)
    {
        uint32_t m = 1 << s; // 2^s
        std::complex<float> wm = std::exp(-2.0f * (float)M_PI * I / (std::complex<float>)m);
        for (int k = 0; k < N; k += m)
        {
            std::complex<float> w = 1.f;
            for (int j = 0; j < m / 2; j++)
            {
                std::complex<float> t = w * output[k + j + m / 2];
                std::complex<float> u = output[k + j];
                output[k + j] = u + t;
                output[k + j + m / 2] = u - t;
                w = w * wm;
            }
        }
    }
}

void rfft(std::complex<float> *input, std::complex<float> *output, uint32_t N)
{
    std::complex<float> I(0.0, 1.0);
    for (int i = 0; i < N / 2; i++)
    {
        input[i] = input[i] + I * input[i + N / 2];
    }

    fft(input, output, N / 2);

    for (int i = 0; i < N / 2; i++)
    {
        output[i] = (output[i] + std::conj(output[(N / 2) - i])) / 2.f;
    }

    for (int i = N / 2; i < N; i++)
    {
        output[i] = -I * (output[i] - std::conj(output[(N / 2) - i])) / 2.f;
    }
}