#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#include "constants.h"
#include "complex.h"
typedef unsigned int uint;

void printTable( uint (*iter_table)[TSIZE_W] ) {
    const int print_align = 3;
    for(int i = 0; i < TSIZE_H; i++ ) {
        for(int j = 0; j < TSIZE_W; j++ ) {
            printf("%*d", print_align, iter_table[i][j]);
        }
        for(int j = 0; j < TSIZE_W - 2; j++ ) {
            printf("%*d", print_align,
                          iter_table[i][TSIZE_W - 2 - j]);
        }
        printf("\n");
    }
}

inline complex getCoords(uint i, uint j) {
    double x_scale = ((double)(i) / (double)(TSIZE_H));
    double y_scale = ((double)(j) / (double)(TSIZE_H));
    complex C;
    C.a  = (4.0 * x_scale) - 2.0;
    C.bi = (4.0 * y_scale) - 2.0;
    return C;
}

inline void getIndeces(complex C, uint *i, uint *j) {
    double x_scale = (C.a  + 2.0) / 4.0;
    double y_scale = (C.bi + 2.0) / 4.0;
    uint x_val = (uint)(x_scale * (double)(TSIZE_H));
    uint y_val = (uint)(y_scale * (double)(TSIZE_H));
    *i = x_val;
    *j = y_val;
}

#endif