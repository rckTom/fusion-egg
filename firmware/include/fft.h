#pragma once

#include <complex>

template <class T>
struct FFT
{
    static void fft(std::complex<T> *samples, std::complex<T> *output, uint32_t N)
    {
        uint8_t log2n = (uint8_t)std::log2(N) + 0.5f;
        std::complex<T> I(0.0, 1.0);
        if (N == 1)
        {
            output[0] = samples[0];
            return;
        }

        // shuffle array
        for (int i = 0; i < N; i++)
        {

            output[i] = samples[FFT::bitReverse(i, log2n)];
        }
        for (int s = 1; s <= log2n; s++)
        {
            uint32_t m = 1 << s; // 2^s
            std::complex<T> wm = std::exp(-2.0f * (T)M_PI * I / (std::complex<T>)m);
            for (int k = 0; k < N; k += m)
            {
                std::complex<T> w = 1.f;
                for (int j = 0; j < m / 2; j++)
                {
                    std::complex<T> t = w * output[k + j + m / 2];
                    std::complex<T> u = output[k + j];
                    output[k + j] = u + t;
                    output[k + j + m / 2] = u - t;
                    w = w * wm;
                }
            }
        }
    }

    static void rfft(std::complex<T> *input, std::complex<T> *output, uint32_t N)
    {
        std::complex<T> I(0.0, 1.0);
        for (int i = 0; i < N / 2; i++)
        {
            input[i] = input[i] + I * input[i + N / 2];
        }

        FFT<T>::fft(input, output, N / 2);

        for (int i = 0; i < N / 2; i++)
        {
            output[i] = (output[i] + std::conj(output[(N / 2) - i])) / 2.;
        }

        for (int i = N / 2; i < N; i++)
        {
            output[i] = -I * (output[i] - std::conj(output[(N / 2) - i])) / 2.;
        }
    }

private:
    static unsigned int bitReverse(unsigned int x, int log2n)
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
};