# build an executable named myprog from myprog.c
all: p_simulator.c
	gcc -g -Wall -o p_simulator p_simulator.c -lm -lcurses -lgsl -lgslcblas

clean:
	$(RM) p_simulator