//
// Created by Yoyotao on 2022/6/2.
//

#ifndef RAYTRACER_NUMERIC_H
#define RAYTRACER_NUMERIC_H

#include <numeric>
#include <cfloat>

class Numeric {
public:
    template<typename T>
    static float Clamp(T val, T a, T b) {
        if (val < a) return a;
        else if (val > b) return b;
        else return val;
    }
};


#endif //RAYTRACER_NUMERIC_H
