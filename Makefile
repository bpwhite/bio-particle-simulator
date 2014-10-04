# build an executable named myprog from myprog.c
all: spart_simulator.c
	gcc -g -Wall -o spart_simulator spart_simulator.c -lm -lcurses -lgsl -lgslcblas

clean:
	$(RM) spart_simulator
