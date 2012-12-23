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
double cur_exp = 0.01;

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

inline void traceBuddhabrot(complex C) {
    uint cur_iter = 0;
    complex Z_i = C;
    uint i, j;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        // Set i and j to array indeces for C in the table
        getIndeces(Z_i, &i, &j);
        // Only increment valid indeces in the half-table
        if(j < TSIZE_W)
            iter_table[i][j]++;
        //Z_i = add(square(Z_i) , C);
        Z_i = add(exponent(Z_i, cur_exp) , C);
        cur_iter++;
    }
}

void fillTableBuddhabrot() {
    int i, j;
    complex C;
    uint cur_value;
    // Iterate across all points on both halves of the plane
    //  as their traces may visit both halves
    for(i = 0; i < TSIZE_H; i++ ) {
        for(j = 0; j < TSIZE_H; j++ ) {
            C = getCoords(i, j);
            cur_value = solveMandelbrot(C);
            // If C is within the threshold values, then it is in
            //  the Buddhabrot set
            if(cur_value <= LEQ_ITER && cur_value >= GEQ_ITER) {
                traceBuddhabrot(C);
            }
        }
    }
}

int main() {
    double t = omp_get_wtime();
    printf("Buddhabrot: \n");
    for(int i = 0; i < 400; i++) {
        fillTableBuddhabrot();
        printTable(iter_table);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

        // gotta clear iter table lol
        for(int i = 0; i < TSIZE_H; i++ )
            for(int j = 0; j < TSIZE_W; j++ )
                iter_table[i][j] = 0;

        cur_exp += 0.01;

        usleep(50000);
    }
    t = omp_get_wtime() - t;
    printf("Elapsed %f seconds.\n", t);
    return 0;
}