#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "complex.h"
typedef unsigned int uint;

#define PRINT_MODE  3
#define PRINT_ALIGN 3

inline void printCur(uint cur_iter) {
    // Aligned integer printing
    if(PRINT_MODE == 1) {
        printf("%*d", PRINT_ALIGN, cur_iter);
    }
    // Logarithmic mandelbrot printing
    else if(PRINT_MODE == 2) {
        char* print_str = " .,xo-~XO0";
        uint  print_num = 10;
        double ratio = log((double)(cur_iter)) / log((double)(MAX_ITER));
        uint index = (uint)( ratio * (double)(print_num - 1) );
        char cur_char = *(print_str + index);
        printf("%c ", cur_char);
    }
    // Logarithmic buddhabrot printing
    else if(PRINT_MODE == 3) {
        char* print_str = " .,xo-~XO0";
        uint  print_num = 10;
        uint  MAX_COUNT = 300;
        double ratio = log((double)(cur_iter)) / log((double)(MAX_COUNT));
        if(ratio > 1.0) ratio = 1.0;
        uint index = (uint)( ratio * (double)(print_num - 1) );
        char cur_char = *(print_str + index);
        printf("%c ", cur_char);
    }
}

void printTable( uint (*iter_table)[TSIZE_W] ) {
    for(int i = 0; i < TSIZE_H; i++ ) {
        for(int j = 0; j < TSIZE_W; j++ ) {
            printCur(iter_table[i][j]);
        }
        for(int j = 0; j < TSIZE_W - 2; j++ ) {
            printCur(iter_table[i][TSIZE_W - 2 - j]);
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