.PHONY: clean

EXEC=tardis
CC=gcc
CFLAGS=
GSL_INC=$(TACC_GSL_INC)
GSL_LIB=$(TACC_GSL_LIB)

all: mytools.c main.c mytools.h
	$(CC) $(CFLAGS) -I$(GSL_INC) main.c mytools.c -lgsl -lgslcblas -L$(GSL_LIB) -o $(EXEC)
clean:
	rm -f *.o tardis
