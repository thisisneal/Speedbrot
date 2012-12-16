#include "complex.h"

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