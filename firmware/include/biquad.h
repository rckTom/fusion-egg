#pragma once

template <class T>
struct Biquad
{
    T b0;
    T b1;
    T b2;
    T a1;
    T a2;
    T xn1;
    T xn2;
    T yn1;
    T yn2;

    Biquad() = default;
    Biquad(T a1, T a2, T b0, T b1, T b2) : b0(b0), b1(b1), b2(b2), a1(a1), a2(a2){};
    Biquad(T a0, T a1, T a2, T b0, T b1, T b2)
    {
        this->a1 = a1 / a0;
        this->a2 = a2 / a0;
        this->b0 = b0 / a0;
        this->b1 = b1 / a0;
        this->b2 = b2 / a0;
    }

    T update(T x)
    {
        T y = this->b0 * x + this->b1 * this->xn1 + this->b2 * this->xn2 - this->yn1 * this->a1 - this->yn2 * this->a2;

        this->xn2 = this->xn1;
        this->xn1 = x;

        this->yn2 = this->yn1;
        this->yn1 = y;

        return y;
    }
};