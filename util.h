#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lodepng.h"

#include "constants.h"
#include "complex.h"
typedef unsigned int uint;

const uint PRINT_MODE  = 3;
const uint PRINT_ALIGN = 3;

const uint PAINT_MODE  = 2;

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
    if(error) printf("error\n");
}

void paintBackground(unsigned char* image, uint pixels, uint argb_color) {
    for(uint cur_pixel = 0; cur_pixel < pixels; cur_pixel++) {
        unsigned char *cur_index = image + (cur_pixel * 4);
        *((uint *)cur_index) = argb_color;
    }
}

uint mixARGB(uint pixelA, uint pixelB) {
    // Thank you dude on stack overflow for figuring this out
    uint aA = (pixelA & 0xFF000000) >> 24;
    uint bA = (pixelA & 0x00FF0000) >> 16;
    uint gA = (pixelA & 0x0000FF00) >>  8;
    uint rA = (pixelA & 0x000000FF);
    uint aB = (pixelB & 0xFF000000) >> 24;
    uint bB = (pixelB & 0x00FF0000) >> 16;
    uint gB = (pixelB & 0x0000FF00) >>  8;
    uint rB = (pixelB & 0x000000FF);
    uint rOut = (rA * aA / 255) + (rB * aB * (255 - aA) / (255*255));
    uint gOut = (gA * aA / 255) + (gB * aB * (255 - aA) / (255*255));
    uint bOut = (bA * aA / 255) + (bB * aB * (255 - aA) / (255*255));
    uint aOut = aA + (aB * (255 - aA) / 255);
    uint pixelOut = (aOut << 24) | (bOut << 16) | (gOut << 8) | (rOut);
    return pixelOut;
}

void colorPixel(unsigned char* image, uint index, uint iter, double frame_pct) {
    // R G B A in memory
    uint value, r_value, g_value, b_value, a_value;
    // Simple buddhabrot grayscale coloring
    if(PAINT_MODE == 1) {
        double ratio = 1.5 * log((double)(iter)) / log((double)(MAX_ITER * 0.25));
        value = (uint)( ratio * 255.0 );
        if(value > 255) value = 255;
        r_value = g_value = b_value = value;
        a_value = 255;

        image[index + 0] = r_value;
        image[index + 1] = g_value;
        image[index + 2] = b_value;
        image[index + 3] = a_value;
    }
    // Colored buddhabrot
    else if(PAINT_MODE == 2) {
        // STARTs determine the RGB color of the cool parts of the image
        const uint R_START = 0xFF * frame_pct;
        const uint G_START = 0x00;
        const uint B_START = 0xFF - 0xFF * frame_pct;
        double ratio = log((double)(iter)) / log((double)(MAX_ITER * 1.5));
        if(ratio < 0.05) {
            r_value = b_value = g_value = a_value = 0;   // black
        } else if(ratio > 1.0) {
            r_value = b_value = g_value = a_value = 255; // white
        } else {
            r_value = R_START + (ratio * (255 - R_START));
            g_value = G_START + (ratio * (255 - G_START));
            b_value = B_START + (ratio * (255 - B_START));
            a_value = (ratio * 255.0);
        }

        uint new_pixel = (a_value << 24) | (b_value << 16) |
                         (g_value <<  8) | (r_value);
        uint *cur_pixel_ptr = (uint *)(image + index);
        uint cur_pixel = *cur_pixel_ptr;
        *cur_pixel_ptr = mixARGB(new_pixel, cur_pixel);
    }
}

void writeImage(uint *iter_table, char* filename, double frame_pctx) {
    /*generate some image*/
    uint width = TSIZE_H, height = TSIZE_H;
    unsigned char* image = malloc(width * height * 4);
    uint background_color = 0xFF000000; // R G B A but endian is backwards
    paintBackground(image, width * height, background_color);

    uint x, y, index, cur_val;
    for(int i = 0; i < TSIZE_H; i++ ) {
        y = i;
        x = 0;
        for(int j = 0; j < TSIZE_W; j++ ) {
            cur_val = iter_table[i * TSIZE_W + j];
            index = 4 * width * y + 4 * x;
            colorPixel(image, index, cur_val, frame_pctx);
            x++;
        }
        for(int j = 0; j < TSIZE_W - 2; j++ ) {
            cur_val = iter_table[i * TSIZE_W + (TSIZE_W - 2 - j)];
            index = 4 * width * y + 4 * x;
            colorPixel(image, index, cur_val, frame_pctx);
            x++;
        }
    }

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