#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>

#include "complex.h"

#define SIZE      256
#define HALF_SIZE (SIZE/2)
#define MAX_ITER  999

typedef unsigned int uint;
uint iter_table[SIZE][HALF_SIZE + 1];

void printTable() {
    const int print_align = 4;
    for(int i = 0; i < SIZE; i++ ) {
        for(int j = 0; j < HALF_SIZE + 1; j++ ) {
            printf("%*d", print_align, iter_table[i][j]);
        }
        for(int j = 0; j < HALF_SIZE - 1; j++ ) {
            printf("%*d", print_align,
                          iter_table[i][HALF_SIZE - 1 - j]);
        }
        printf("\n");
    }
}

// Return whether the point C is contained within a circle
//  of radius 2 about the origin.
inline bool inSet(complex C) {
    double magnitude = C.a * C.a + C.bi * C.bi;
    return (magnitude < 4.0);
}

inline uint solveMandelbrot(complex C) {
    uint cur_iter = 0;
    complex Z_i = C;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        Z_i = add(square(Z_i) , C);
        cur_iter++;
    }
    return cur_iter;
}

inline complex getCoords(uint i, uint j) {
    double x_scale = ((double)(i) / (double)(SIZE));
    double y_scale = ((double)(j) / (double)(SIZE));
    complex C;
    C.a  = (4.0 * x_scale) - 2.0;
    C.bi = (4.0 * y_scale) - 2.0;
    return C;
}

void fillTable() {
    int i, j;
    complex C;
    uint cur_value;
    #pragma omp parallel for private(i, j, C, cur_value) shared(iter_table) schedule(dynamic)
    for(i = 0; i < SIZE; i++ ) {
        for(j = 0; j < HALF_SIZE + 1; j++ ) {
            C = getCoords(i, j);
            cur_value = solveMandelbrot(C);
            iter_table[i][j] = cur_value;
        }
    }
}

int main() {
    double t = omp_get_wtime();
    printf("Mandlebrot: \n");
    fillTable();
    //printTable();
    t = omp_get_wtime() - t;
    printf("Elapsed %f seconds.\n", t);
    return 0;
}