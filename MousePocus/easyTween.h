#pragma once
#include <math.h>

inline float easeOutSin(float t, float b, float f, float d) {
    // inout
    float c = f - b;
//  return -c / 2 * (cos( PI * t / d) - 1) + b;
    
    // in
    //return -c * cos(t / d * (PI / 2)) + c + b;

    // out
    return (f - b) * sinf((t / d) * (PI / 2)) + b;
}

inline float easeOutExp(float t, float b, float f, float d) {
    //inout
    //return -c / 2 * (cos( PI * t / d) - 1) + b;

    //in
    //return -c * cos(t / d * (PI / 2)) + c + b;

    float c = f - b;
    if (t >= d) {
        return b + c;
    }

    return b + c * (1 - powf(2, -10 * t / d));


}

inline float easeInExp(float t, float b, float f, float d) {
    float c = f - b;
    if (d == 0) return b + c;
    return (t == 0) ? b : c * powf(2, 10 * (t / d - 1)) + b;
}