#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdbool.h>
#include <math.h>

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

/*
 * Get the magnitude (r) of the complex number
 *  where r^2 = (a^2 + bi^2)
 */
inline double getRadius(complex C) {
    return sqrt((C.a * C.a) + (C.bi * C.bi));
}

inline complex exponent(complex C, double n) {
    double r = getRadius(C);                 // radius r
    double theta = atan2(C.bi, C.a);
    double r_prod = pow(r, n);                  // r^n
    double n_theta = n * theta;                 // n*theta
    complex prod;
    prod.a  = r_prod * cos(n_theta);
    prod.bi = r_prod * sin(n_theta);
    return prod;
}

// Return whether the point C is contained within a circle
//  of radius 2 about the origin.
inline bool inSet(complex C) {
    double magnitude = C.a * C.a + C.bi * C.bi;
    return (magnitude < 4.0);
}

#endif