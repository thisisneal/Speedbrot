CC=gcc
CFLAGS=-I -g -Wall -std=gnu99 -O2 -fopenmp
MDEPS = mandelbrot.c complex.c complex.h

mandelbrot: $(MDEPS)
	$(CC) $(MDEPS) $(CFLAGS) -o mandelbrot

clean:
	rm mandelbrot mandelbrot.o