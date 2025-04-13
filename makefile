all:
	gcc -O2 hacksaw.c bytearray/bytearray.c -I./bytearray -lm -o hacksaw
