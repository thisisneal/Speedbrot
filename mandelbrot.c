#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>

#include "complex.h"
#include "constants.h"
#include "util.h"

uint iter_table[TSIZE_H][TSIZE_W];

inline uint solveMandelbrot(complex C) {
    uint cur_iter = 0;
    complex Z_i = C;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        Z_i = add(square(Z_i) , C);
        cur_iter++;
    }
    return cur_iter;
}

void fillTableMandelbrot() {
    int i;
    #pragma omp parallel for private(i) \
                             shared(iter_table) \
                             schedule(dynamic)
    for(i = 0; i < TSIZE_H; i++ ) {
        for(uint j = 0; j < TSIZE_W; j++ ) {
            complex C = getCoords(i, j);
            uint cur_value = solveMandelbrot(C);
            iter_table[i][j] = cur_value;
        }
    }
}

int main() {
    double t = omp_get_wtime();
    printf("Mandlebrot: \n");
    fillTableMandelbrot();
    t = omp_get_wtime() - t;
    printf("Calculations done in %f seconds.\n", t);
    double t2 = omp_get_wtime();
    //printTable(iter_table);
    //writeImage(iter_table, "images/mandelbrot03.png");
    t2 = omp_get_wtime() - t2;
    printf("Drawing done in %f seconds.\n", t2);
    return 0;
}