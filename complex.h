#ifndef COMPLEX_H
#define COMPLEX_H

struct complexObj {
    double a;
    double bi;
};
typedef struct complexObj complex;

complex add(complex L, complex R);
complex mult(complex L, complex R);
complex square(complex C);

#endif