CC=gcc
CFLAGS=-I -g -Wall -std=gnu99 -O2 -fopenmp -lm
MDEPS = lodepng.h lodepng.c constants.h complex.h util.h mandelbrot.c
BDEPS = lodepng.h lodepng.c constants.h complex.h util.h buddhabrot.c

default: mandelbrot buddhabrot

mandelbrot: $(MDEPS)
	$(CC) $(MDEPS) -o mandelbrot $(CFLAGS)

buddhabrot: $(BDEPS)
	$(CC) $(BDEPS) -o buddhabrot $(CFLAGS)

clean:
	rm mandelbrot
	rm buddhabrot