
all:
	gcc -O3 -o msr_reader msr_reader.c

object:
	gcc -O3 -c msr_reader.c