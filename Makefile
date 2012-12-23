CC=gcc
CFLAGS=-I -g -Wall -std=gnu99 -O2 -fopenmp
MDEPS = constants.h complex.h util.h mandelbrot.c
BDEPS = constants.h complex.h util.h buddhabrot.c

default: mandelbrot buddhabrot

mandelbrot: $(MDEPS)
	$(CC) $(MDEPS) $(CFLAGS) -o mandelbrot

buddhabrot: $(BDEPS)
	$(CC) $(BDEPS) $(CFLAGS) -o buddhabrot

clean:
	rm mandelbrot
	rm buddhabrot