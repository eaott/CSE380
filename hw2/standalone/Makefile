.PHONY: clean

EXEC=tardis
CC=gcc
CFLAGS=-O0

all: mytools.c main.c mytools.h
	@echo $(EXEC)
	touch dumb.o
	@echo $(TACC_GSL_INC)
	$(CC) -I$(TACC_GSL_INC) main.c mytools.c -lgsl -lgslcblas -L$(TACC_GSL_LIB) -o $(EXEC)
clean:
	rm -f *.o tardis
