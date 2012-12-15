#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>

#define SIZE      24
#define MAX_ITER  999

struct complexObj {
    double a;
    double bi;
};
typedef struct complexObj complex;
typedef unsigned int uint;
uint iter_table[SIZE][SIZE];

void printTable() {
    for(int i = 0; i < SIZE; i++ ) {
        for(int j = 0; j < SIZE; j++ ) {
            printf("%*d", 3, iter_table[i][j]);
        }
        printf("\n");
    }
}

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

// Return whether the point C is contained within a circle
//  of radius 2 about the origin.
inline bool inSet(complex C) {
    double magnitude = C.a * C.a + C.bi * C.bi;
    return (magnitude < 4.0);
}

inline uint solveMandelbrot(complex C) {
    uint cur_iter = 1;
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
        for(j = 0; j < SIZE; j++ ) {
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
    printTable();
    t = omp_get_wtime() - t;
    printf("Elapsed %f seconds.\n", t);
    return 0;
}