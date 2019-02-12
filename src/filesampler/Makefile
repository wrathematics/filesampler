CC = gcc
CFLAGS = -fopenmp -O3 -std=c99 -Wall -Wno-unused-function

OBJECTS = file_sampler.o wc.o

all: shlib

shlib: clean
	$(CC) $(CFLAGS) -shared -o libfilesampler.so $(OBJECTS)

clean:
	rm -f ./libfilesampler.so
