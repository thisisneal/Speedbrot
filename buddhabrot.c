#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#include "complex.h"
#include "constants.h"
#include "util.h"

const double start_exp = 1.01;
const double end_exp   = 2.0;
const uint   frames    = 300;

const char *base_filepath = "images/buddhabrot15/";

inline uint solveMandelbrot(complex C, double cur_exp) {
    uint cur_iter = 0;
    complex Z_i = C;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        Z_i = add(exponent(Z_i, cur_exp) , C);
        cur_iter++;
    }
    return cur_iter;
}

inline void traceBuddhabrot(complex C, double cur_exp, uint *iter_table) {
    uint cur_iter = 0;
    complex Z_i = C;
    uint i, j;
    while(cur_iter < MAX_ITER && inSet(Z_i)) {
        // Set i and j to array indeces for C in the table
        getIndeces(Z_i, &i, &j);
        // Only increment valid indeces in the half-table
        if(j < TSIZE_W)
            iter_table[i * TSIZE_W + j]++;
        Z_i = add(exponent(Z_i, cur_exp) , C);
        cur_iter++;
    }
}

void fillTableBuddhabrot(double cur_exp, uint *iter_table) {
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

void clearTable(uint *iter_table) {
    for(int i = 0; i < TSIZE_H; i++ )
        for(int j = 0; j < TSIZE_W; j++ )
            iter_table[i * TSIZE_W + j] = 0;
}

void getFilename(uint i, char *filename) {
    char cur_img_num[5] = "";
    sprintf(cur_img_num, "%d", i);
    strcat(filename, base_filepath);
    if(i < 100) strcat(filename, "0");
    if(i < 10 ) strcat(filename, "0");
    strcat(filename, cur_img_num);
    strcat(filename, ".png");
}

int main() {
    char info_file_name[60];
    strcat(info_file_name, base_filepath);
    strcat(info_file_name, "info.txt");
    FILE *file;
    file = fopen(info_file_name, "w+");
    fprintf(file, "Size of Buddhabrot: %d\n", TSIZE_H);
    fprintf(file, "Max iterations: %d\n", MAX_ITER);
    fprintf(file, "Number of frames: %d\n", frames);
    fprintf(file, "Exponent Range: %f - %f\n", start_exp, end_exp);
    fclose(file);

    double begin_t = omp_get_wtime();
    double exp_inc = (end_exp - start_exp) / (double)(frames - 1);
    printf("Buddhabrot: \n");

    int i;
    #pragma omp parallel for private(i) \
                             schedule(dynamic)
    for(i = 0; i < frames; i++) {
        double start_t = omp_get_wtime();

        uint *iter_table = calloc(TSIZE_W * TSIZE_H, sizeof(uint));
        if(iter_table == NULL)
            printf("MALLOC ERROR!!\n");

        double f_pct = (double) i / frames;
        // Iteration on a scale of 0 - 1 with unit spline
        double exp_pct = -2.0 * f_pct * f_pct * f_pct + 3.0 * f_pct * f_pct;
        double cur_exp = start_exp + (end_exp - start_exp) * exp_pct;

        // linear exponent increase
        //double cur_exp = start_exp + exp_inc * i;
        fillTableBuddhabrot(cur_exp, iter_table);

        char cur_file_name[60] = "";
        getFilename(i, cur_file_name);
        writeImage(iter_table, cur_file_name, (double)i / frames);

        free(iter_table);

        printf("Completed frame : %*d ", 3, i);
        double new_t = omp_get_wtime();
        printf("in %6.3f seconds.\n", new_t - start_t);
    }
    double total_t = omp_get_wtime() - begin_t;
    printf("Elapsed %f seconds.\n", total_t);

    return 0;
}