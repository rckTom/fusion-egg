#pragma once

template <class T>
struct DcCancelation
{
    T x_n1;
    T y_n1;
    T R;

    DcCancelation(T R) : R(R){};

    T update(T x)
    {
        T y = x - this->x_n1 + this->R * this->y_n1;
        this->x_n1 = x;
        this->y_n1 = y;

        return y;
    };
};