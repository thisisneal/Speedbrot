#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#include "complex.h"
#include "constants.h"
#include "util.h"

uint iter_table[TSIZE_H][TSIZE_W];
double cur_exp = 0.1;

inline uint solveMandelbrot(complex C) {
    uint cur_iter = 0;
    complex Z_i = C;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        //Z_i = add(square(Z_i) , C);
        Z_i = add(exponent(Z_i, cur_exp) , C);
        cur_iter++;
    }
    return cur_iter;
}

void fillTableMandelbrot() {
    int i, j;
    complex C;
    uint cur_value;
    #pragma omp parallel for private(i, j, C, cur_value) \
                             shared(iter_table) \
                             schedule(dynamic)
    for(i = 0; i < TSIZE_H; i++ ) {
        for(j = 0; j < TSIZE_W; j++ ) {
            C = getCoords(i, j);
            cur_value = solveMandelbrot(C);
            iter_table[i][j] = cur_value;
        }
    }
}

int main() {
    double t = omp_get_wtime();
    printf("Mandlebrot: \n");
    // fillTableMandelbrot();
    // printTable(iter_table);
    for(int i = 0; i < 70; i++) {
        fillTableMandelbrot();
        printTable(iter_table);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

        // gotta clear iter table lol
        for(int i = 0; i < TSIZE_H; i++ )
            for(int j = 0; j < TSIZE_W; j++ )
                iter_table[i][j] = 0;

        cur_exp += 0.1;

        usleep(100000);
    }
    t = omp_get_wtime() - t;
    printf("Elapsed %f seconds.\n", t);
    return 0;
}