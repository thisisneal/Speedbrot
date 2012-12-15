CC=gcc
CFLAGS=-I -g -Wall -std=gnu99 -O2 -fopenmp

mandelbrot: mandelbrot.o
	$(CC) $(CFLAGS) -o mandelbrot mandelbrot.o -I.