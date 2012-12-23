#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdbool.h>

struct complexObj {
    double a;
    double bi;
};
typedef struct complexObj complex;

inline complex add(complex L, complex R) {
    complex sum;
    sum.a  = L.a  + R.a;
    sum.bi = L.bi + R.bi;
    return sum;
}

inline complex mult(complex L, complex R) {
    complex prod;
    prod.a  = (L.a * R.a ) - (L.bi * R.bi);
    prod.bi = (L.a * R.bi) + (L.bi * R.a );
    return prod;
}

inline complex square(complex C) {
    return mult(C, C);
}

// Return whether the point C is contained within a circle
//  of radius 2 about the origin.
inline bool inSet(complex C) {
    double magnitude = C.a * C.a + C.bi * C.bi;
    return (magnitude < 4.0);
}

#endif