#pragma once

template <class T>
struct Pt1
{
    T y_n1;
    T K;
    T T1;

    /* PT1: y = y_(n-1) + (Ku - y_(n-1)) * dt/T1 */
    Pt1(T K, T T1) : T1(T1), K(K){};
    Pt1() = default;
    T update(T u, T dt)
    {
        T y = this->y_n1 + (this->K * u - this->y_n1) * dt / this->T1;
        this->y_n1 = y;
        return y;
    }

    void reset() {
        this->y_n1 = 0.f;
    }
};