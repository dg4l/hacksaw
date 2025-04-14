objects = tinyfiledialogs.o hacksaw.o bytearray.o

all: hacksaw

hacksaw: $(objects)
	gcc $(objects) -g -O2 -lm -Wall -o hacksaw

tinyfiledialogs.o: ./tfd/tinyfiledialogs.c
	gcc -c ./tfd/tinyfiledialogs.c

bytearray.o: ./bytearray/bytearray.c
	gcc -c ./bytearray/bytearray.c

hacksaw.o: hacksaw.c
	gcc -c hacksaw.c -g -I./bytearray -I./tfd

.PHONY: all
