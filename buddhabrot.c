#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#include "complex.h"
#include "constants.h"
#include "util.h"

uint iter_table_A[TSIZE_H][TSIZE_W];
uint iter_table_B[TSIZE_H][TSIZE_W];

inline uint solveMandelbrot(complex C, double cur_exp) {
    uint cur_iter = 0;
    complex Z_i = C;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        Z_i = add(exponent(Z_i, cur_exp) , C);
        cur_iter++;
    }
    return cur_iter;
}

inline void traceBuddhabrot(complex C, double cur_exp, uint (*iter_table)[TSIZE_W]) {
    uint cur_iter = 0;
    complex Z_i = C;
    uint i, j;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        // Set i and j to array indeces for C in the table
        getIndeces(Z_i, &i, &j);
        // Only increment valid indeces in the half-table
        if(j < TSIZE_W)
            iter_table[i][j]++;
        Z_i = add(exponent(Z_i, cur_exp) , C);
        cur_iter++;
    }
}

void fillTableBuddhabrot(double cur_exp, uint (*iter_table)[TSIZE_W]) {
    int i, j;
    complex C;
    uint cur_value;
    // Iterate across all points on both halves of the plane
    //  as their traces may visit both halves
    for(i = 0; i < TSIZE_H; i++ ) {
        for(j = 0; j < TSIZE_H; j++ ) {
            C = getCoords(i, j);
            cur_value = solveMandelbrot(C, cur_exp);
            // If C is within the threshold values, then it is in
            //  the Buddhabrot set
            if(cur_value <= LEQ_ITER && cur_value >= GEQ_ITER) {
                traceBuddhabrot(C, cur_exp, iter_table);
            }
        }
    }
}

void clearTable(uint (*iter_table)[TSIZE_W]) {
    for(int i = 0; i < TSIZE_H; i++ )
            for(int j = 0; j < TSIZE_W; j++ )
                iter_table[i][j] = 0;
}

void getFilename(uint i, char *filename) {
    char cur_img_num[5] = "";
    sprintf(cur_img_num, "%d", i);
    strcat(filename, "images/buddhabrot10/frame");
    if(i < 100) strcat(filename, "0");
    if(i < 10 ) strcat(filename, "0");
    strcat(filename, cur_img_num);
    strcat(filename, ".png");
}

int main() {
    double start_t = omp_get_wtime();
    double prev_t  = start_t;

    double start_exp = 1.0;
    double end_exp   = 2.0;
    const uint frames = 10;
    double exp_inc   = (end_exp - start_exp) / (double)(frames);

    printf("Buddhabrot: \n");

    for(int i = 0; i < (frames / 2) + 1; i++) {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                uint frame_a = 2 * i;
                double exp_a = start_exp + exp_inc * frame_a;
                fillTableBuddhabrot(exp_a, iter_table_A);

                char cur_file_name_A[60] = "";
                getFilename(frame_a, cur_file_name_A);
                writeImage(iter_table_A, cur_file_name_A);

                clearTable(iter_table_A);
            }
            #pragma omp section
            {
                uint frame_b = (2 * i) + 1;
                double exp_b = start_exp + exp_inc * frame_b;
                fillTableBuddhabrot(exp_b, iter_table_B);

                char cur_file_name_B[60] = "";
                getFilename(frame_b, cur_file_name_B);
                writeImage(iter_table_B, cur_file_name_B);

                clearTable(iter_table_B);
            }
        }
        printf("Completed index : %*d ", 3, i);
        double new_t = omp_get_wtime();
        printf("in %f seconds.\n", new_t - prev_t);
        prev_t = new_t;
    }
    double total_t = omp_get_wtime() - start_t;
    printf("Elapsed %f seconds.\n", total_t);
    return 0;
}