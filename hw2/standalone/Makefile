.PHONY: clean

all: mytools.c main.c mytools.h
	gcc -I$TACC_GSL_INC main.c mytools.c -lgsl -lgslcblas -L$TACC_GSL_LIB -o tardis

clean:
	rm *.o
	rm tardis
