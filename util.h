#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lodepng.h"

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

// Lodepng example direct image writing
void encodeOneStep(const char* filename, const unsigned char* image,
                   unsigned width, unsigned height)
{
    /*Encode the image*/
    unsigned error = lodepng_encode32_file(filename, image, width, height);

    if(error) printf("error");
}

void writeImage(char* filename) {
    /*generate some image*/
    uint width = TSIZE_H, height = TSIZE_H;
    unsigned char* image = malloc(width * height * 4);
    uint x, y;
    for(y = 0; y < height; y++) {
        for(x = 0; x < width; x++) {
            image[4 * width * y + 4 * x + 0] = 255;
            image[4 * width * y + 4 * x + 1] = x % 255;
            image[4 * width * y + 4 * x + 2] = x % 255;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }

    /*run an example*/
    encodeOneStep(filename, image, width, height);

    free(image);
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